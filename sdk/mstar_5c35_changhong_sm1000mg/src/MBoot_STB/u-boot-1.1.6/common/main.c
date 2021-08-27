/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Add to readline cmdline-editing by
 * (C) Copyright 2005
 * JinHua Luo, GuangDong Linux Center, <luo.jinhua@gd-linux.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* #define	DEBUG	*/

#include <common.h>
#include <config.h>
#include <watchdog.h>
#include <command.h>
#include <malloc.h>
#include <usb.h>
#include <fat.h>
#include <ms_ver.h>
#include <msKeypad.h>
#include <environment.h>
#include "uboot_mmap.h"
#include <sys/common/MsCommon.h>
#include <sys/common/MsOS.h>
#include <sys/drv/drvSERFLASH.h>

#if (ENABLE_HUMMINGBIRD_CUSTOMER)

#include "../oad/cus_oad/apiRfChannel.h"
#include "../drivers/device/fp/drvFP.h"
#include "../drivers/apiDTVSatTable.h"
#include "apiDigiTuner.h"
#include "frontend_config.h"

#if ENABLE_DVB_S_SYS
#include <device/MSB131x.h>
#endif

#endif

#if defined (CONFIG_MODEM_SUPPORT)
#include <malloc.h>		/* for free() prototype */
#endif

#if (CONFIG_COMMANDS & CFG_CMD_BOOTLOGO)
#include <display_logo/display_main.h>
#endif

#if defined (CFG_HUSH_PARSER)
#include <hush.h>
#endif

#include <post.h>

#if (CONFIG_COMMANDS & CFG_CMD_SECURE)
#include <sys/common/MsTypes.h>
#include <sys/common/MsDevice.h>
#include <secure/crypto_auth.h>
#endif

#if (CONFIG_COMMANDS & CFG_CMD_EEPROM)
#include <sys/common/MsTypes.h>
#include <sys/drv/drvGPIO.h>
DECLARE_GLOBAL_DATA_PTR;
#endif

#if (CONFIG_SILENT_CONSOLE)
DECLARE_GLOBAL_DATA_PTR;
#endif

#include <sys/common/MsTypes.h>
#include <secure/apiSecureBoot.h>
#include <sys/drv/drvBDMA.h>

#define MemAlign(n, unit)           ((((n)+(unit)-1)/(unit))*(unit))
#define	endtick(seconds) (get_ticks() + (uint64_t)(seconds) * get_tbclk())
#define LINUX_KERNEL_BOOT_PARAMETER_PARTITION_SIZE 0x4000
#define SYS_INFO_NAND_LEN       0x400   // 1k
#define PANEL_INFO_NAND_LEN     0x400   // 1k
#define TEMP_BUFFER_ADDR        0xA0400000 //use KERNEL address temporarily
#define MAX_DELAY_STOP_STR      32
// Global Function
#define BUF_SIZE 4096
#if(ENABLE_MODULE_WDT_ENABLE)
#define WDT_ENABLE_HB 1
#else
#define WDT_ENABLE_HB 0
#endif

#if WDT_ENABLE_HB
#include <sys/drv/drvWDT.h>
#endif

#if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
#define GWIN_WIDTH              720
#define GWIN_HEIGHT             576
#define GRAPHIC_WIDTH           600
#define GRAPHIC_HEIGHT          400
#define GRAPHIC_X               60
#define GRAPHIC_Y               88
#define LINE_HEIGHT             50
#define RECT_LEFT_INTERVAL      50
#endif

#ifndef CONFIG_BOOT_RETRY_MIN
#define CONFIG_BOOT_RETRY_MIN CONFIG_BOOT_RETRY_TIME
#endif
#define CFG_OAD_IN_MBOOT "OAD_IN_MBOOT"

#define ATSC_SDDS_ENABLE                0               // 0: DVB OAD mode; 1: ATSC SDDS mode.
#if ATSC_SDDS_ENABLE
#define BASEADDRESS_SDDS_INFO_IN_FLASH  0x00700000      // Sync this address with HB's definition.
#endif

#ifndef BOOL
typedef unsigned int BOOL;
#endif

typedef enum ENCRW_STATUS
{
    NCRW_STS_OK             = 0,
    NCRW_STS_NOMEDIA        = 0x1,
    NCRW_STS_PROGRAM_FAIL   = 0x2,
    NCRW_STS_ECC_ERROR      = 0x4,
    NCRW_STS_WRITE_PROTECT  = 0x8,
    NCRW_STS_TIMEOUT        = 0x10,
    NCRW_STS_ADDRESS_ERROR  = 0x20
}NCRW_STATUS;

typedef struct
{
    U16 u16HStart;
    U16 u16VStart;
    U16 u16Width;
    U16 u16Height;
    U16 u16HTotal;
    U16 u16VTotal;
    U16 u16DE_VStart;
    U32 u32DClkFactor;
    U16 u16DefaultVFreq;

    // LPLL
    U16 u16LPLL_InputDiv;
    U16 u16LPLL_LoopDiv;
    U16 u16LPLL_OutputDiv;
    U8 u8LPLL_Type;
    U8 u8LPLL_Mode;

    // sync
    U8 u8HSyncWidth;
    U8 bPnlDblVSync; //org: BOOL bPnlDblVSync;

    // output control
    U16 u16OCTRL;
    U16 u16OSTRL;
    U16 u16ODRV;
    U16 u16DITHCTRL;

    // MOD
    U16 u16MOD_CTRL0;
    U16 u16MOD_CTRL9;
    U16 u16MOD_CTRLA;
    U8 u8MOD_CTRLB;

    //titania to URSA
	U8  u8LVDSTxChannel; //ursa scaler
	U8  u8LVDSTxBitNum; //ursa scaler
	U8  u8LVDSTxTiMode;  //ursa scaler 40-bit2
	U8  u8LVDSTxSwapMsbLsb; //ursa scaler
	U8  u8LVDSTxSwap_P_N; //ursa scaler
	U8  u8LVDSTxSwapOddEven; //ursa scaler

	//URSA to Panel info
	U8  u8PanelVfreq; //ursa scaler
	U8  u8PanelChannel; //ursa scaler
	U8  u8PanelLVDSSwapCH; //ursa scaler
	U8  u8PanelBitNum; //ursa scaler
	U8  u8PanelLVDSShiftPair; //ursa scaler
	U8  u8PanelLVDSTiMode; //ursa scaler
	U8  u8PanelLVDSSwapPol; //ursa scaler
	U8  u8PanelLVDSSwapPair; //ursa scaler

	// additional for Chakra (Obama doens't have these member yet)
	U8  u8VSyncWidth;
	U16 u16LVDSTxSwapValue;

	U32 dclk_mode;
    U16 u16MinHTotal;
    U16 u16DefaultHTotal;
    U16 u16MaxHTotal;
	U16 u16MinVTotal;
	U16 u16DefaultVTotal;
	U16 u16MaxVTotal;
	U32 u32MinDCLK;
	U32 u32MaxDCLK;
}PANEL_INFO;

typedef struct
{
    U8 displayControllerSeperated;
    U8 local_ir;
    U8 gop_index;
}SYS_INFO;

typedef struct
{
    U32 u32PanelInfoAddr;
    U32 u32PanelInfoLen;
    U32 u32SysInfoAddr;
    U32 u32SysInfoLen;
    U32 u32ChakraBinAddr;
    U32 u32ChakraBinLen;
}BOOT_PARAM;

#if ATSC_SDDS_ENABLE
typedef struct
{
    MS_BOOL bFoundNewSDDS_4BL_Download;     //TRUE:  SDDS is available; FALSE: NO/Old.
    MS_U32  u32SDDS_Frequency;      // Frequency: 32 bits (Decimal). e.g. 474000    //MS_FE_CARRIER_PARAM
    MS_U8   u8SDDS_Bandwidth;       // Bandwidth: 1: 6MHz, 2: 7MHz, 3: 8MHz
    MS_U16  u16SDDS_PID;            // Pid: 16 bits (Hex) pid, e.g. 0x150
    MS_U32  u32SDDS_OUI;            // OUI: 32 bits (Hex) ID, e.g. 0x000214
    MS_U16  u16SDDS_HW_ModelID;     // HW ID: 16 bits (Hex) ID, e.g. 0x0001
    MS_U16  u16SDDS_HW_Verion;      // HW Version number: 16 bits (Hex), e.g. 0x0001
    MS_U16  u16SDDS_SW_ModelID;     // SW ID: 16 bits (Hex) ID, e.g. 0x0001
    MS_U16  u16SDDS_SW_Verion;      // SW Version number: 16 bits (Hex), e.g. 0x0001
    MS_U8   u8ScheduleDescCount;    // 0: immediate;
    MS_U32  u32SchStartTime;
    MS_U32  u32SchLengthInSeconds;
    //MS_SDDS_SCHEDUL_TIME_INFO   *msScheduleTimers;
} MS_SDDS_PARAM_2_BL;

static MS_BOOL  MApp_LoadSDDSInfoFromFlash(MS_SDDS_PARAM_2_BL *SDDSinfo);
static MS_BOOL  MApp_SaveSDDSInfoIntoFlash(MS_SDDS_PARAM_2_BL *_SDDSinfo);
#endif

char console_buffer[CFG_CBSIZE];		/* console I/O buffer	*/

#ifndef CONFIG_CMDLINE_EDITING
static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen);
static char erase_seq[] = "\b \b";		/* erase sequence	*/
static char   tab_seq[] = "        ";		/* used to expand TABs	*/
#endif /* CONFIG_CMDLINE_EDITING */

#if defined (CONFIG_BOOT_RETRY_TIME)
static uint64_t endtime = 0;  /* must be set, default is instant timeout */
static int      retry_time = -1; /* -1 so can call readline before main_loop */
#endif

static int parse_line (char *, char *[]);
#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
static int abortboot(int);
#endif

#if defined(CONFIG_BOOT_RETRY_TIME) && defined(CONFIG_RESET_TO_RETRY)
extern int do_reset (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);		/* for do_reset() prototype */
#endif
extern int do_bootd (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
extern NCRW_STATUS MDrv_NAND_Load_IMAGE(U32 u32DRAMADDR);
extern U8 MDrv_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum);
extern U8 MDrv_MIU_SetGroupPriority_UBoot(U8 bMIU1, U8 *au8GpPriority, U8 u8GpNum);
extern char *getenv (char *name);
extern int strcmp(const char * cs,const char * ct);
#if defined (CONFIG_MODEM_SUPPORT)
int do_mdm_init = 0;
extern void mdm_init(void); /* defined in board.c */
#endif

#if (CONFIG_COMMANDS & CFG_CMD_EEPROM)
extern void MDrv_SwIIC_Init(void);
#endif

static int s_repeatable = 0;
static U8 s_usbUpgrade = 0;

void set_miu4boot(void);
static void set_miu(void);
#if defined(CONFIG_USB_STORAGE)
static void check_usb_upgrade(void);
#endif
static void set_info_exchange_cfg(void);
static void set_env_cfg_2bootargs(void);
#if defined (CONFIG_OAD)
static void update_bootarg(void);
#endif //defined (CONFIG_OAD)

#if defined(CONFIG_DISPLAY_LOGO)
static void update_bootlogo_bootarg(void);
#endif

/***************************************************************************
 * Watch for 'delay' seconds for autoboot stop or autoboot delay string.
 * returns: 0 -  no key string, allow autoboot
 *          1 - got key string, abort
***************************************************************************/
#if defined (CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
#if defined (CONFIG_AUTOBOOT_KEYED)
static __inline__ int abortboot(int bootdelay)
{
	int abort = 0;
	uint64_t etime = endtick(bootdelay);
	struct {
		char* str;
		u_int len;
		int retry;
	}
	delaykey [] = {
		{ str: getenv ("bootdelaykey"),  retry: 1 },
		{ str: getenv ("bootdelaykey2"), retry: 1 },
		{ str: getenv ("bootstopkey"),   retry: 0 },
		{ str: getenv ("bootstopkey2"),  retry: 0 },
	};

	char presskey [MAX_DELAY_STOP_STR];
	u_int presskey_len = 0;
	u_int presskey_max = 0;
	u_int i;

#if (CONFIG_SILENT_CONSOLE)
	if (gd->flags & GD_FLG_SILENT)
    {
		console_assign (stdout, "serial");
		console_assign (stderr, "serial");
	}
#endif

#  ifdef CONFIG_AUTOBOOT_PROMPT
	printf (CONFIG_AUTOBOOT_PROMPT, bootdelay);
#  endif

#  ifdef CONFIG_AUTOBOOT_DELAY_STR
	if (delaykey[0].str == NULL)
		delaykey[0].str = CONFIG_AUTOBOOT_DELAY_STR;
#  endif
#  ifdef CONFIG_AUTOBOOT_DELAY_STR2
	if (delaykey[1].str == NULL)
		delaykey[1].str = CONFIG_AUTOBOOT_DELAY_STR2;
#  endif
#  ifdef CONFIG_AUTOBOOT_STOP_STR
	if (delaykey[2].str == NULL)
		delaykey[2].str = CONFIG_AUTOBOOT_STOP_STR;
#  endif
#  ifdef CONFIG_AUTOBOOT_STOP_STR2
	if (delaykey[3].str == NULL)
		delaykey[3].str = CONFIG_AUTOBOOT_STOP_STR2;
#  endif

	for (i = 0; i < sizeof(delaykey) / sizeof(delaykey[0]); i ++) {
		delaykey[i].len = delaykey[i].str == NULL ?
				    0 : strlen (delaykey[i].str);
		delaykey[i].len = delaykey[i].len > MAX_DELAY_STOP_STR ?
				    MAX_DELAY_STOP_STR : delaykey[i].len;

		presskey_max = presskey_max > delaykey[i].len ?
				    presskey_max : delaykey[i].len;

#  if DEBUG_BOOTKEYS
		printf("%s key:<%s>\n",
		       delaykey[i].retry ? "delay" : "stop",
		       delaykey[i].str ? delaykey[i].str : "NULL");
#  endif
	}

	/* In order to keep up with incoming data, check timeout only
	 * when catch up.
	 */
	while (!abort && get_ticks() <= etime) {
		for (i = 0; i < sizeof(delaykey) / sizeof(delaykey[0]); i ++) {
			if (delaykey[i].len > 0 &&
			    presskey_len >= delaykey[i].len &&
			    memcmp (presskey + presskey_len - delaykey[i].len,
				    delaykey[i].str,
				    delaykey[i].len) == 0) {
#  if DEBUG_BOOTKEYS
				printf("got %skey\n",
				       delaykey[i].retry ? "delay" : "stop");
#  endif

#  ifdef CONFIG_BOOT_RETRY_TIME
				/* don't retry auto boot */
				if (! delaykey[i].retry)
					retry_time = -1;
#  endif
				abort = 1;
			}
		}

		if (tstc()) {
			if (presskey_len < presskey_max) {
				presskey [presskey_len ++] = getc();
			}
			else {
				for (i = 0; i < presskey_max - 1; i ++)
					presskey [i] = presskey [i + 1];

				presskey [i] = getc();
			}
		}
	}
#  if DEBUG_BOOTKEYS
	if (!abort)
		puts ("key timeout\n");
#  endif

#if (CONFIG_SILENT_CONSOLE)
	if (abort)
    {
		/* permanently enable normal console output */
		gd->flags &= ~(GD_FLG_SILENT);
	}
    else if (gd->flags & GD_FLG_SILENT)
	{
		/* Restore silent console */
		console_assign (stdout, "nulldev");
		console_assign (stderr, "nulldev");
	}
#endif

	return abort;
}

# else	/* !defined(CONFIG_AUTOBOOT_KEYED) */

#if defined (CONFIG_MENUKEY)
static int menukey = 0;
#endif



static __inline__ int abortboot(int bootdelay)
{
	int abort = 0;
    int ch = 0;
#if (CONFIG_SILENT_CONSOLE)
	if (gd->flags & GD_FLG_SILENT)
    {
		console_assign (stdout, "serial");
		console_assign (stderr, "serial");
	}
#endif

#if defined (CONFIG_MENUPROMPT)
	printf(CONFIG_MENUPROMPT, bootdelay);
#else
#if (!CONFIG_MSTAR_SILENT)
	printf("Hit any key to stop autoboot: %2d ", bootdelay);
#endif
#endif

#if (CONFIG_ZERO_BOOTDELAY_CHECK)
#if (CONFIG_SILENT_CONSOLE)
    if (gd->flags & GD_FLG_SILENT)
    {
		/* Restore silent console */
		console_assign (stdout, "nulldev");
		console_assign (stderr, "nulldev");
	}
#endif
	/*
	 * Check if key already pressed
	 */
	if (bootdelay == 0)
    {
		if (tstc())
        {	/* we got a key press	*/
			ch = getc();  /* consume input	*/
			puts ("\b\b\b 0");
		    if (ch == 0x1B)//when esc,upgrade flash.bin
			{

				setenv("USBUpdateFlag","1");
				setenv("USBUpgradeFlag", "1");
				saveenv();
				return 0;
			}
			abort = 1; 	/* don't auto boot	*/
#if (CONFIG_SILENT_CONSOLE)
            gd->flags &= ~GD_FLG_SILENT;
            puts("Disable Silent Mode\n");
#endif
		}
	}
#endif

	while ((bootdelay > 0) && (!abort))
    {
		int i;

		--bootdelay;
		/* delay 100 * 10ms */
		for (i=0; !abort && i<100; ++i)
        {
			if (tstc())
            {	/* we got a key press	*/
				abort  = 1;	/* don't auto boot	*/
				bootdelay = 0;	/* no more delay	*/
# ifdef CONFIG_MENUKEY
				menukey = getc();
# else
				(void) getc();  /* consume input	*/
# endif
				break;
			}
			udelay (10000);
		}

		printf ("\b\b\b%2d ", bootdelay);
	}

#if (!CONFIG_MSTAR_SILENT)
	putc ('\n');
#endif
	return abort;
}
# endif	/* CONFIG_AUTOBOOT_KEYED */
#endif	/* CONFIG_BOOTDELAY >= 0  */
int mtdparts_init(void);
/****************************************************************************/
extern int usb_stor_scan(int mode);
int fileNum = 0,lookup = 0 ,jffs2Num=0;

#if (CONFIG_COMMANDS & CFG_CMD_SECURE)
#define NONPM_BASE          0xBF200000
#define REG_MBX_EMPTY		(NONPM_BASE+(0x1980<<2))
#define	U32                 unsigned long int

#define REG(addr)            (*(volatile U32 *)(addr))

void HAL_Write2Byte(U32 u32REG,U16 u16dat)
{
    REG(u32REG) = u16dat;
}

void Secure_Switch(void)
{
	HAL_Write2Byte(REG_MBX_EMPTY+(0x6D*4), 0x5AA5);
}
#endif

/**********************************************************************************************************************/
///This function to filterate char then leave number and dot(e.g:0.0.28)
/**********************************************************************************************************************/
#if defined (CONFIG_SW_VERSION) && defined(CONFIG_USB_STORAGE)
static int Filtrate_OtherChar(char *buf)
{
  int i=0;
  char * temp=NULL;
  temp = buf;
  int value=0;
  value= strlen(buf);
  for(i=0;i<value;i++)
    {
      if(((*temp>=48)&&(*temp<=57))||(*temp==46)||(*temp==86))
        temp++;
      else
        {
         value= i;
         break;
        }
    }
  return value;
}

void check_sw_version(void)
{
    char *cmd=NULL,*sw_version=NULL,*temp_store=NULL,*sw_version_buffer=NULL;
    int compare_value,check_version,number=0;
    sw_version_buffer = (char *)malloc(20);
    cmd =(char*)malloc(50);
    temp_store = (char*)malloc(60);
    //check memory while or not be distributed
    if(sw_version_buffer!=NULL)
    memset(sw_version_buffer, 0, 20);
    else
    {
      printf("error:>malloc fail memory!\n");
      return ;
    }
    if(temp_store!=NULL)
    memset(temp_store,0,60);
    else
    {
      printf("error:>malloc fail memory!\n");
      return ;
    }
    if(cmd==NULL)
    {
      printf("error:>malloc fail memory!\n");
      return ;
    }
    //check usb port0 & port1
    if(usb_init(0)>=0)
    {
        if(usb_stor_scan(1)!=-1)
        {
            sprintf(cmd, "fatload usb 0  0x%x  /Target/version.txt",sw_version_buffer);
            run_command(cmd,0);
        }
    }
    else if(usb_init(1)>=0)
    {
        if(usb_stor_scan(1)!=-1)
        {
            sprintf(cmd, "fatload usb 0  0x%x  /Target/version.txt",sw_version_buffer);
            run_command(cmd,0);
        }
    }

    //Get env of sw_version
    sw_version= getenv("sw_version");
    if(sw_version!=NULL)
        memcpy(temp_store,sw_version,60);
    compare_value=strcmp(sw_version_buffer,"");
    if(compare_value!=0)
    {
        number=Filtrate_OtherChar(sw_version_buffer);
        if(number>0)
        {
            sw_version_buffer[number]='\0';
        }
        //filtrate Other char
        number=0;
        number=Filtrate_OtherChar(temp_store);
        if(number>0)
        {
            temp_store[number]='\0';
        }

        printf("sw_version:%s\n",sw_version);
        printf("strlen(sw_version) is:%d\n",strlen(sw_version));
        printf("temp_store:%s\n",temp_store);

        check_version=strcmp(temp_store,sw_version_buffer);
        printf("check_version is:%d\n", check_version);
        if(cmd!=NULL)
        {
            printf("start free cmd!\n");
            free(cmd);
            cmd=NULL;
        }
        if(temp_store!=NULL)
        {
            printf("start free temp_store!\n");
            free(temp_store);
            temp_store=NULL;
        }
        if(check_version!=0)
        {
            run_command("ustar",0);

            printf("start write buffer:%s\n", sw_version_buffer);
            setenv("sw_version", sw_version_buffer);
            printf("start save sw_version!\n");
            saveenv();
            printf("end save sw_version!\n");
            if(sw_version_buffer != NULL)
            {
                printf("start free buffer!\n");
                free(sw_version_buffer);
                sw_version_buffer=NULL;
            }

        }
    }
     if(cmd!=NULL)
        {
            printf("start free cmd_1!\n");
            free(cmd);
        }
        if(temp_store!=NULL)
        {
            printf("start free temp_store_1!\n");
            free(temp_store);
        }
     if(sw_version_buffer != NULL)
       {
            printf("start free buffer_1!\n");
            free(sw_version_buffer);
       }

}
#endif

#if defined (CONFIG_KEYPAD)
extern 	U8 msKeypad_Get_ADC_Channel(U8 Channel, U8 *pvalue);
void config_keypad(U8 *keypad_pressed)
{
	U8 i=0;
    #if defined (CONFIG_POWER_KEY)
    U8 j=0;
    for(i=0;i<ADC_KEY_CHANNEL_NUM;i++)
	{
	    U8 adc_val=0xFF;
	    msKeypad_Get_ADC_Channel(i,&adc_val);
	    if(adc_val==0x0)
	    {
	        printf("start delay 3m: ......\n");
	        udelay(3*1000*1000);
            for(j=0;j<ADC_KEY_CHANNEL_NUM;j++)
                {
                 msKeypad_Get_ADC_Channel(j,&adc_val);
                 if(adc_val==0x0)
                    {
                       *keypad_pressed=1;
	                   break;
                    }
                }
             break;
	    }

	}
    #else
	for(i=0;i<ADC_KEY_CHANNEL_NUM;i++)
	{
	    U8 adc_val=0xFF;
	    msKeypad_Get_ADC_Channel(i,&adc_val);
	    if(adc_val<ADC_KEY_LEVEL_MAX)
	    {
	        *keypad_pressed=1;
	        break;
	    }

	}
  #endif

}
#endif

#if defined (CONFIG_WR_FLAG)
#if (RM_EEPROM_TYPE == RM_TYPE_24C512 )
#define RM_MAX_ADDRESS  (0xffff)
#elif (RM_EEPROM_TYPE == RM_TYPE_24C16 )
#define RM_MAX_ADDRESS  (0x07ff)
#elif (RM_EEPROM_TYPE == RM_TYPE_24C32 )
#define RM_MAX_ADDRESS  (0x0fff)
#elif (RM_EEPROM_TYPE == RM_TYPE_24C64)
#define RM_MAX_ADDRESS (0x1fff)
#endif

#define RM_SW_UPDATE_ADDRESS     (RM_MAX_ADDRESS-8)
extern int usb_max_devs;
int Upgrade_By_Reeprom(void)
{
  ulong	addr;
  char	linebuf[16];
#if defined (CONFIG_USB_STORAGE)
  char cmd_flag[50];
  char cmd[50];
  ulong num=0;
#endif
  ulong default_set;
  char * switch_to_int=NULL;
  char *buffer=NULL;
  unsigned char  *version=NULL,*temp_version=NULL;
  u_char	*ucp = (u_char *)linebuf;

  switch_to_int= (char*)malloc(10);
  version = (unsigned char*)malloc(10);
  buffer = (char*)malloc(20);

  if(version!=NULL)
    temp_version = version;

  memset(linebuf,0,16);
  #if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_HX)
  run_command("eeprom read 0x80200000 0x1FFA 0x1",0);
  #elif(MS_BOARD_TYPE_SEL == BD_MST072D_SKYWORTH)
  run_command("eeprom read 0x80200000 0x1FE3 0x1",0);
  #else
  run_command("eeprom read 0x80200000 0xFFFF 0x1",0);
  #endif
  run_command("md 0x80200000 0x1",0);

  addr = simple_strtoul("0x80200000", NULL, 16);
  *ucp = *((u_char *)addr);
  printf("\ndata_1[%x]--->:%02x\n\n", (RM_SW_UPDATE_ADDRESS+3),(*ucp = *((u_char *)addr)));
  default_set = simple_strtoul("a5",NULL,16);
  printf("defatult_set is:%ld\n",default_set);
  printf("*ucp is:%d\n",*ucp);
  if(*ucp==default_set)
    {
       printf("update start------------>\n");
       //update start
       #if defined (CONFIG_USB_STORAGE)
       int usb_p_switch =-1;
       if(usb_init(0)>=0)
        usb_p_switch=0;
       else if(usb_init(1)>=0)
        usb_p_switch=1;
       printf("usb_p_switch is:%d\n",usb_p_switch);
       if(usb_p_switch==0||usb_p_switch==1)
       {
         if(usb_stor_scan(1)!=-1)
         {
            run_command("ustar",0);

            //write version to env
            sprintf(cmd, "fatload usb 0  0x%x  /Target/version.txt",buffer);
            run_command(cmd,0);
            printf("start write buffer:%s\n",buffer);
            setenv("sw_version",buffer);
            printf("start save sw_version!\n");
            saveenv();

            //write falg to eeprom
            printf("start write eeprom--------->!\n");
            memset(version,255,10);
            memset(cmd_flag,0,50);
		  	#if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_HX)
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FFA 0x1",&version[1]);
            #elif(MS_BOARD_TYPE_SEL == BD_MST072D_SKYWORTH)
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FE3 0x1",&version[1]);
			#else
            sprintf(cmd_flag, "eeprom write 0x%x 0xFFFF 0x1",&version[1]);
			#endif
            run_command(cmd_flag,0);
            printf("end write eeprom---------->!\n");

            //write version to eeprom
            memset(version,0,10);
            version = (unsigned char*)getenv("sw_version");
            printf("version_2 is:%s\n",version);

            memset(switch_to_int,0,10);
            switch_to_int[0] =version[6];
            switch_to_int[1] =version[5];
            num= simple_strtoul(switch_to_int,NULL,16);
		  	#if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_HX)
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FF9 0x1",&num);
            #elif(MS_BOARD_TYPE_SEL == BD_MST072D_SKYWORTH)
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FE2 0x1",&num);
			#else
            sprintf(cmd_flag, "eeprom write 0x%x 0xFFFE 0x1",&num);
			#endif
            run_command(cmd_flag,0);

            memset(switch_to_int,0,10);
            switch_to_int[0] =version[3];
            num= simple_strtoul(switch_to_int,NULL,16);
		  	#if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_HX)
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FF8 0x1",&num);
            #elif(MS_BOARD_TYPE_SEL == BD_MST072D_SKYWORTH)
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FE1 0x1",&num);
			#else
            sprintf(cmd_flag, "eeprom write 0x%x 0xFFFD 0x1",&num);
			#endif
            run_command(cmd_flag,0);

            memset(switch_to_int,0,10);
            switch_to_int[0] =version[1];
            num= simple_strtoul(switch_to_int,NULL,16);
		  	#if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_HX)
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FF7 0x1",&num);
            #elif(MS_BOARD_TYPE_SEL == BD_MST072D_SKYWORTH)
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FE0 0x1",&num);
			#else
	        sprintf(cmd_flag, "eeprom write 0x%x 0xFFFC 0x1",&num);
			#endif
            run_command(cmd_flag,0);
         }
       }
       #endif
      }
    if(switch_to_int!=NULL)
        free(switch_to_int);
    if(temp_version!=NULL)
    {
        free(temp_version);
    }
    if(buffer!=NULL)
    {
        free(buffer);
    }
    return 0;
}
int upgrade_by_rweeprom_havehub(void)
{
  ulong	addr,addr1,default_set,default_set1;
  char	linebuf[16],linebuf1[16];

#if defined (CONFIG_USB_STORAGE)
  char cmd_flag[50], cmd[50];
  ulong num=0;
#endif

  int startFlag=-1;
  unsigned char  *version=NULL,*temp_version=NULL;
  u_char	*ucp = (u_char *)linebuf,*ucp1 = (u_char *)linebuf1;
  char *buffer=NULL,* switch_to_int=NULL;

  switch_to_int= (char*)malloc(10);
  version = (unsigned char*)malloc(10);
  buffer = (char*)malloc(20);

  if(version!=NULL)
    temp_version = version;

  memset(linebuf,0,16);
  #if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_TCL)//address is: 0x1FFB 0x1FFA 0x1FF9 0x1FF8  0x1FF7
  {
  run_command("eeprom read 0x80200000 0x1FFA 0x1",0);
  run_command("eeprom read 0x80400000 0x1FFB 0x1",0);
  }
  #endif

  //get value from sdram
  addr = simple_strtoul("0x80200000", NULL, 16);//a5
  addr1 = simple_strtoul("0x80400000", NULL, 16);//5a
  *ucp = *((u_char *)addr);//a5
  *ucp1 = *((u_char *)addr1);//5a
  printf("\ndata_1[1FFA]--->:%02x\n\n", (*ucp = *((u_char *)addr)));
  printf("\ndata_2[1FFB]--->:%02x\n\n", (*ucp1 = *((u_char *)addr1)));
  //display and switch value
  default_set = simple_strtoul("a5",NULL,16);
  default_set1 = simple_strtoul("5a",NULL,16);
  printf("---------->defatult_set is:%ld\n",default_set);
  printf("---------->defatult_set1 is:%ld\n",default_set1);
  printf("*ucp is:%d\n",*ucp);
  printf("*ucp1 is:%d\n",*ucp1);
  //judge while or not upgrade
  #if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_TCL)
    {
      if((*ucp!=default_set)||(*ucp1!=default_set1))
        {
         startFlag =1;
         printf("startFlag_0 is:%d\n",startFlag);
        }
    }
  #endif
    printf("startFlag_2 is>>>>>>>>>>>>>>:%d\n",startFlag);
    if(startFlag==1)
    {
       printf("update start------------>\n");
       //update start
       #if defined (CONFIG_USB_STORAGE)

        #if 0
        int usb_p_switch =-1;
        if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_TCL)
        {
             if(usb_init(1)>=0){
                  usb_p_switch=1;}
             else if(usb_init(0)>=0){
                 usb_p_switch=0;}
             if(usb_p_switch==0||usb_p_switch==1)
                {
                  printf("usb_p_switch is------@1------>>:%d\n",usb_p_switch);
                  udelay(2000*1000);
                  if(usb_p_switch==1){
                     run_command("usb start 1", 0);}
                  else{
                    run_command("usb start 0", 0);}
                }
        }
        #endif

         #if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_TCL)
          {
                run_command("usb start 1", 0);
                if(usb_max_devs<1)
                {
                    run_command("usb start 0", 0);
                }
          }
         #endif

       if(usb_max_devs>=1)
       {
            run_command("ustar",0);

            //write version to env
            sprintf(cmd, "fatload usb 0  0x%x  /Target/version.txt",buffer);
            run_command(cmd,0);
            printf("start write buffer:%s\n",buffer);
            setenv("sw_version",buffer);
            printf("start save sw_version!\n");
            saveenv();

            //write falg to eeprom
            memset(version,255,10);
            memset(cmd_flag,0,50);
		  	#if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_TCL)
            {
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FFA 0x1",&version[1]);
            run_command(cmd_flag,0);
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FFB 0x1",&version[1]);
            run_command(cmd_flag,0);
            }
			#endif

            //write version to eeprom,and get version from env
            memset(version,0,10);
            version = (unsigned char*)getenv("sw_version");
            printf("version_2 is:%s\n",version);

            //write version of [5-6]
            memset(switch_to_int,0,10);
            switch_to_int[0] =version[6];
            switch_to_int[1] =version[5];
            num= simple_strtoul(switch_to_int,NULL,16);
		  	#if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_TCL)
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FF9 0x1",&num);
            run_command(cmd_flag,0);
			#endif


            //write version of [3]
            memset(switch_to_int,0,10);
            switch_to_int[0] =version[3];
            num= simple_strtoul(switch_to_int,NULL,16);
		  	#if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_TCL)
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FF8 0x1",&num);
            run_command(cmd_flag,0);
			#endif


            //write version of [1]
            memset(switch_to_int,0,10);
            switch_to_int[0] =version[1];
            num= simple_strtoul(switch_to_int,NULL,16);
		  	#if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_TCL)
            sprintf(cmd_flag, "eeprom write 0x%x 0x1FE7 0x1",&num);
            run_command(cmd_flag,0);
			#endif

       }
       #endif
      }

    if(switch_to_int!=NULL){
        free(switch_to_int);}
    if(temp_version!=NULL){
        free(temp_version);}
    if(buffer!=NULL){
        free(buffer);}
    return 0;
}
#endif


#if defined(CONFIG_KRONUS)||defined(CONFIG_URANUS4) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA) || defined(CONFIG_KELTIC)|| defined(CONFIG_KRITI)|| defined(CONFIG_KERES)
#if (CONFIG_PM)
#define STB_UBOOT_INFO_ADDR               0x40000
#define STB_UBOOT_ROM_END_ADDR            0x40008
#define STB_UBOOT_LOGO_ROM_END_ADDR       0x40024
#define STB_INFO_SIZE                     0x10000
#else
#define STB_UBOOT_INFO_ADDR               0x10000
#define STB_UBOOT_ROM_END_ADDR            0x10008
#define STB_UBOOT_LOGO_ROM_END_ADDR       0x10024
#define STB_INFO_SIZE                     0x10000
#endif

#if (CONFIG_USB)
extern MS_BOOL MDrv_SERFLASH_Read(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer);



U8 STB_USB_AutoUpdate(void)
{
    char buffer[256];

    int ret;
    U8 *pTemp;
    U32 u32AppMagicFlag;
    U32 CRC_result=0;
    U32 CRC_value=0;
    U32 addr=0;
    U32 file_size=0;
    U32 u32UbootInfoAddr;
    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
    int progress_cnt;
    #endif
    SERFLASH_Info *pflash_info = NULL;



    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
    sprintf(buffer, "osd_create %d %d", GWIN_WIDTH, GWIN_HEIGHT);
    run_command(buffer, 0);
    sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
    run_command(buffer, 0);
    sprintf(buffer, "draw_string %d %d 0x3fffffff 1 LOADING DATA...", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
    run_command(buffer, 0);
    progress_cnt = 0;
    sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
    run_command(buffer, 0);
    #endif



    memset(buffer, 0 , 256);
    MsOS_Dcache_Flush((U32)UPGRADE_BUFFER_ADDR,(U32)0x40000);
    MsOS_FlushMemory();

    pflash_info = (SERFLASH_Info *)MDrv_SERFLASH_GetInfo();
    sprintf(buffer, "fatload usb 0 %X %s 0x%X", UPGRADE_BUFFER_ADDR, "/flash.bin",pflash_info->u32TotalSize);


    ret = run_command(buffer, 0);
    if(ret!=0)
    {
        printf("cmd %s failed\n", buffer);
        //return FALSE;
        goto ErrorExit;
    }
    else
    {
        printf("cmd %s success\n", buffer);
    }

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
    if (progress_cnt < 100)
    {
        progress_cnt += 20;
        sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 5, GRAPHIC_WIDTH, LINE_HEIGHT);
        run_command(buffer, 0);
        sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
        run_command(buffer, 0);
    }
    #endif

    pTemp = (U8 *)((U32)UPGRADE_BUFFER_ADDR);
    pTemp = pTemp + STB_UBOOT_LOGO_ROM_END_ADDR;

    // u32UbootInfoAddr
    u32UbootInfoAddr = 0;
    u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp));
    pTemp ++;
    u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp) << 8);
    pTemp++;
    u32UbootInfoAddr = u32UbootInfoAddr | (((*pTemp) & (0xF)) << 16);

    if(u32UbootInfoAddr == 0)
    {
        pTemp = (U8 *)((U32)UPGRADE_BUFFER_ADDR);
        pTemp = pTemp + STB_UBOOT_ROM_END_ADDR;

        // u32UbootInfoAddr
        u32UbootInfoAddr = 0;
        u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp));
        pTemp ++;
        u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp) << 8);
        pTemp++;
        u32UbootInfoAddr = u32UbootInfoAddr | (((*pTemp) & (0xF)) << 16);
    }

    printf("u32UbootInfoAddr: 0x%lx\n", u32UbootInfoAddr);

    u32UbootInfoAddr = u32UbootInfoAddr + 65535;
    // To get the first APP magic flag which is at uboot_size(align to 0x10000) + STB_UBOOT_INFO_ADDR(sboot size)
    u32UbootInfoAddr = ((u32UbootInfoAddr >> 16) << 16) + STB_UBOOT_INFO_ADDR;

    printf("Boot_INFO_ADDR: 0x%lx\n", u32UbootInfoAddr);

    pTemp = (U8 *)((U32)UPGRADE_BUFFER_ADDR);
    //pTemp = pTemp + STB_INFO_ADDR;
    pTemp = pTemp + u32UbootInfoAddr;

    // MagicFlag
    u32AppMagicFlag = 0;
    u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 24);
    pTemp ++;
    u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 16);
    pTemp++;
    u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 8);
    pTemp++;
    u32AppMagicFlag = u32AppMagicFlag | ((*pTemp));

    if(u32AppMagicFlag != 0xDEADBEEF)
    {
        printf("AppMagicFlag ERROR !!\n");
        //return FALSE;
        goto ErrorExit;
    }
    else
    {
        printf("AppMagicFlag is correct !!\n");
    }

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
    if (progress_cnt < 100)
    {
        progress_cnt += 20;
        sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 5, GRAPHIC_WIDTH, LINE_HEIGHT);
        run_command(buffer, 0);
        sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
        run_command(buffer, 0);
    }
    #endif

    file_size = simple_strtol(getenv("filesize"), NULL, 16);
    if((file_size <= 4) || (file_size%4 != 0))
    //if(file_size <= 4)
    {
        //wrong file size
        printf("wrong file size: 0x%lx\n", file_size);
        //return FALSE;
        goto ErrorExit;
    }

    addr = UPGRADE_BUFFER_ADDR+(file_size-4);
    CRC_value = CRC_result+1;
    if(addr > UPGRADE_BUFFER_ADDR)
    {
        CRC_value=*((U32*)(addr));
    }
    CRC_result=crc32( 0x00, (U8*)UPGRADE_BUFFER_ADDR, file_size-4 );
    if(CRC_result!= CRC_value)
    {
        //CRC error
        printf("CRC check error !!\n");
        //return FALSE;
        goto ErrorExit;
    }
    else
    {
        printf("CRC check success !!\n");
    }

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
    if (progress_cnt < 100)
    {
        progress_cnt += 20;
        sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 5, GRAPHIC_WIDTH, LINE_HEIGHT);
        run_command(buffer, 0);
        sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
        run_command(buffer, 0);
    }
    #endif


    memset(buffer, 0 , 256);
    sprintf(buffer, "%lX", file_size);
	setenv("filesize", buffer);

    file_size = file_size + 65535;
    file_size = ((file_size >> 16)<<16);
    memset(buffer, 0 , 256);
    sprintf(buffer,"spi_wrc 0x%x 0x00000 0x%x", UPGRADE_BUFFER_ADDR,file_size);
    printf("%s\n",buffer);
    printf("0\n");

    ret = run_command(buffer, 0);

    if(ret!=0)
    {
        printf("cmd %s failed\n", buffer);
        //return FALSE;
        goto ErrorExit;
    }
    else
    {
        printf("cmd %s success\n", buffer);
    }

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
    progress_cnt = 100;
    sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
    run_command(buffer, 0);
    sprintf(buffer, "draw_string %d %d 0x3fffffff 1 SOFTWARE UPGRADE COMPLETE", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
    run_command(buffer, 0);
    sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
    run_command(buffer, 0);
    udelay(1000000);
    sprintf(buffer, "osd_destroy"); //clear screen
    run_command(buffer, 0);

    #endif
    return TRUE;

ErrorExit:

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
    sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
    run_command(buffer, 0);
    sprintf(buffer, "draw_string %d %d 0x3fffffff 1 SOFTWARE UPGRADE ERROR", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
    run_command(buffer, 0);
    //udelay(5000000);
    //sprintf(buffer, "reset");
    //run_command(buffer, 0);

    #endif

    return FALSE;

}
#endif


U8 STB_Read_Boot_info(void)
{
    char buffer[256];
    U8 *pTemp;
    U32 u32AppMagicFlag;
    U32 u32AppFileLen;
    U32 u32AppDramAddr;
    U32 u32AppRestAddr;
    U32 u32AppSPIAddr;
    U32 u32LzmaBufAddr;
    U32 u32LzmaDecBufAddr;
    U32 u32UbootInfoAddr;
    int ret;


    memset(buffer, 0 , 256);

    sprintf(buffer,"spi_rdc 0x80B00000 0x%x 0x%x", STB_UBOOT_INFO_ADDR, STB_INFO_SIZE);
    printf("%s\n",buffer);
    //printf("0\n");

    ret = run_command(buffer, 0);

    if(ret!=0)
    {
        printf("cmd %s failed\n", buffer);
        return FALSE;
    }
    else
    {
        printf("cmd %s success\n", buffer);
    }

    pTemp = (U8 *)((U32)0x80B00000+(STB_UBOOT_LOGO_ROM_END_ADDR-STB_UBOOT_INFO_ADDR));

    // u32UbootInfoAddr
    u32UbootInfoAddr = 0;
    u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp));
    pTemp ++;
    u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp) << 8);
    pTemp++;
    u32UbootInfoAddr = u32UbootInfoAddr | (((*pTemp) & (0xF)) << 16);

    if(u32UbootInfoAddr == 0)
    {
        pTemp = (U8 *)((U32)0x80B00000+(STB_UBOOT_ROM_END_ADDR-STB_UBOOT_INFO_ADDR));

        // u32UbootInfoAddr
        u32UbootInfoAddr = 0;
        u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp));
        pTemp ++;
        u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp) << 8);
        pTemp++;
        u32UbootInfoAddr = u32UbootInfoAddr | (((*pTemp) & (0xFF)) << 16);
    }

    printf("u32UbootInfoAddr: 0x%lx\n", u32UbootInfoAddr);

    u32UbootInfoAddr = u32UbootInfoAddr + 65535;
    // To get the first APP magic flag which is at uboot_size(align to 0x10000) + STB_UBOOT_INFO_ADDR(sboot size)
    u32UbootInfoAddr = ((u32UbootInfoAddr >> 16) << 16) + STB_UBOOT_INFO_ADDR;

    printf("BOOT_INFO_ADDR: 0x%lx\n", u32UbootInfoAddr);


    memset(buffer, 0 , 256);

    //sprintf(buffer,"spi_rdc 0x80B00000 0x%x 0x%x", STB_INFO_ADDR, STB_INFO_SIZE);
    sprintf(buffer,"spi_rdc 0x80B00000 0x%x 0x%x", u32UbootInfoAddr, STB_INFO_SIZE);

    printf("%s\n",buffer);
    //printf("0\n");

    ret = run_command(buffer, 0);

    if(ret!=0)
    {
        printf("cmd %s failed\n", buffer);
        return FALSE;
    }
    else
    {
        printf("cmd %s success\n", buffer);
    }

    //printf("1\n");
    pTemp = (U8 *)((U32)0x80B00000);

    // MagicFlag
    u32AppMagicFlag = 0;
    u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 24);
    pTemp ++;
    u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 16);
    pTemp++;
    u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 8);
    pTemp++;
    u32AppMagicFlag = u32AppMagicFlag | ((*pTemp));

    if(u32AppMagicFlag != 0xDEADBEEF)
    {
        printf("AppMagicFlag ERROR !!\n");
        return FALSE;
    }

    //printf("2\n");
    // AppFileLen
    u32AppFileLen = 0;
    pTemp ++;
    u32AppFileLen = u32AppFileLen | ((*pTemp) << 24);
    pTemp ++;
    u32AppFileLen = u32AppFileLen | ((*pTemp) << 16);
    pTemp++;
    u32AppFileLen = u32AppFileLen | ((*pTemp) << 8);
    pTemp++;
    u32AppFileLen = u32AppFileLen | ((*pTemp));

    // u32AppDramAddr
    u32AppDramAddr = 0;
    pTemp ++;
    u32AppDramAddr = u32AppDramAddr | ((*pTemp) << 24);
    pTemp ++;
    u32AppDramAddr = u32AppDramAddr | ((*pTemp) << 16);
    pTemp++;
    u32AppDramAddr = u32AppDramAddr | ((*pTemp) << 8);
    pTemp++;
    u32AppDramAddr = u32AppDramAddr | ((*pTemp));

    // u32AppRestAddr
    u32AppRestAddr = 0;
    pTemp ++;
    u32AppRestAddr = u32AppRestAddr | ((*pTemp) << 24);
    pTemp ++;
    u32AppRestAddr = u32AppRestAddr | ((*pTemp) << 16);
    pTemp++;
    u32AppRestAddr = u32AppRestAddr | ((*pTemp) << 8);
    pTemp++;
    u32AppRestAddr = u32AppRestAddr | ((*pTemp));

    // u32AppSPIAddr
    u32AppSPIAddr = 0;
    pTemp ++;
    u32AppSPIAddr = u32AppSPIAddr | ((*pTemp) << 24);
    pTemp ++;
    u32AppSPIAddr = u32AppSPIAddr | ((*pTemp) << 16);
    pTemp++;
    u32AppSPIAddr = u32AppSPIAddr | ((*pTemp) << 8);
    pTemp++;
    u32AppSPIAddr = u32AppSPIAddr | ((*pTemp));


    // u32LzmaBufAddr
    u32LzmaBufAddr = 0;
    pTemp ++;
    u32LzmaBufAddr = u32LzmaBufAddr | ((*pTemp) << 24);
    pTemp ++;
    u32LzmaBufAddr = u32LzmaBufAddr | ((*pTemp) << 16);
    pTemp++;
    u32LzmaBufAddr = u32LzmaBufAddr | ((*pTemp) << 8);
    pTemp++;
    u32LzmaBufAddr = u32LzmaBufAddr | ((*pTemp));


    // u32LzmaDecBufAddr
    u32LzmaDecBufAddr = 0;
    pTemp ++;
    u32LzmaDecBufAddr = u32LzmaDecBufAddr | ((*pTemp) << 24);
    pTemp ++;
    u32LzmaDecBufAddr = u32LzmaDecBufAddr | ((*pTemp) << 16);
    pTemp++;
    u32LzmaDecBufAddr = u32LzmaDecBufAddr | ((*pTemp) << 8);
    pTemp++;
    u32LzmaDecBufAddr = u32LzmaDecBufAddr | ((*pTemp));


    //printf("3\n");
    memset(buffer, 0 , 256);

    //sprintf(buffer, "setenv bootcmd ' spi_rdc 0x80B00000 6001C 300000; LzmaDec 0x80B00000 0x300000 0x80000180 0x81000000; go 80000224;");

    //sprintf(buffer, "setenv bootcmd ' spi_rdc 0x%x 0x%x 0x%x; LzmaDec 0x%x 0x%x 0x%x 0x%x; go 0x%x;",u32LzmaBufAddr,u32AppSPIAddr,u32AppFileLen,u32LzmaBufAddr,u32AppFileLen,u32AppDramAddr,u32LzmaDecBufAddr,u32AppRestAddr);

    //u32AppFileLen = 0x300000;
    #if defined(CONFIG_BOOT_SPI_LINUX)
    sprintf(buffer, "setenv bootcmd ' usb exit;spi_rdc 0x%x 0x%x 0x%x; go 0x%x;", u32LzmaBufAddr, u32AppSPIAddr, u32AppFileLen, u32LzmaBufAddr);
    #else
    sprintf(buffer, "setenv bootcmd ' usb exit;spi_rdc 0x%x 0x%x 0x%x; LzmaDec 0x%x 0x%x 0x%x 0x%x; go 0x%x;",u32LzmaBufAddr,u32AppSPIAddr,u32AppFileLen,u32LzmaBufAddr,u32AppFileLen,u32AppDramAddr,u32LzmaDecBufAddr,u32AppRestAddr);
    #endif


    //printf("4\n");
    printf("%s\n",buffer);

    ret = run_command (buffer, 0);

    if(ret!=0)
    {
        printf("cmd %s failed\n", buffer);
        return FALSE;
    }
    else
    {
        printf("cmd %s success\n", buffer);
    }


    memset(buffer, 0 , 256);

    sprintf(buffer, "saveenv");


    ret = run_command (buffer, 0);

    if(ret!=0)
    {
        printf("cmd %s failed\n", buffer);
        return FALSE;
    }
    else
    {
        printf("cmd %s success\n", buffer);
    }

    return TRUE;
}


#if (CONFIG_OAD)&(CONFIG_OAD_IN_MBOOT)

#define OAD_NONE        "0"
#define OAD_DONE        "1"
#define OAD_FAIL        "2"
#define OAD_WRITING_NVM "3"

#define OAD_TRUE    "1"
#define OAD_FALSE   "0"
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#define APP_SIZE 0x400000
//#define APP_INFO_ADDRESS 0x70000
#define APP_INFO_ADDRESS 0xA0000
//#define APP_INFO_ADDRESS 0x90000
//#define APP_INFO_ADDRESS 0x40000
#define APP_INFO_SIZE 0x100
U8 NormalAPPEntry(void)
{

    char pcmd_buffer[256];
    MS_U8 *pAppBuffer = NULL;
    MS_U8 *pTemp = NULL;
    MS_U32 appsize = 0;

    U32 u32AppDramAddr;
    U32 u32AppRestAddr;
    U32 u32AppSPIAddr;
    U32 u32LzmaBufAddr;
    U32 u32LzmaDecBufAddr;
    //U32 u32UbootInfoAddr;
    MS_U32 appimage = 0;
    pAppBuffer = malloc(APP_SIZE);
    if(!pAppBuffer)
    {
        return 0;
    }

    sprintf(pcmd_buffer,"spi_rdc 0x80B00000 0x%x 0x%x",APP_INFO_ADDRESS,APP_INFO_SIZE);
    run_command(pcmd_buffer,0);

    pTemp = (U8 *)((U32)0x80B00000);

    appimage = appimage | ((*pTemp) << 24);
    pTemp ++;
    appimage = appimage | ((*pTemp) << 16);
    pTemp++;
    appimage = appimage | ((*pTemp) << 8);
    pTemp++;
    appimage = appimage | ((*pTemp));
    if(appimage != 0xDEADBEEF)
    {
        printf("\napp info head error!!!\n");
        return 0;
    }

    appsize = 0;
    pTemp++;
    appsize = appsize | ((*pTemp) << 24);
    pTemp ++;
    appsize = appsize | ((*pTemp) << 16);
    pTemp++;
    appsize = appsize | ((*pTemp) << 8);
    pTemp++;
    appsize = appsize | ((*pTemp));

    u32AppDramAddr = 0;
    pTemp ++;
    u32AppDramAddr = u32AppDramAddr | ((*pTemp) << 24);
    pTemp ++;
    u32AppDramAddr = u32AppDramAddr | ((*pTemp) << 16);
    pTemp++;
    u32AppDramAddr = u32AppDramAddr | ((*pTemp) << 8);
    pTemp++;
    u32AppDramAddr = u32AppDramAddr | ((*pTemp));

    u32AppRestAddr = 0;
    pTemp ++;
    u32AppRestAddr = u32AppRestAddr | ((*pTemp) << 24);
    pTemp ++;
    u32AppRestAddr = u32AppRestAddr | ((*pTemp) << 16);
    pTemp++;
    u32AppRestAddr = u32AppRestAddr | ((*pTemp) << 8);
    pTemp++;
    u32AppRestAddr = u32AppRestAddr | ((*pTemp));

    u32AppSPIAddr = 0;
    pTemp ++;
    u32AppSPIAddr = u32AppSPIAddr | ((*pTemp) << 24);
    pTemp ++;
    u32AppSPIAddr = u32AppSPIAddr | ((*pTemp) << 16);
    pTemp++;
    u32AppSPIAddr = u32AppSPIAddr | ((*pTemp) << 8);
    pTemp++;
    u32AppSPIAddr = u32AppSPIAddr | ((*pTemp));

    u32LzmaBufAddr = 0;
    pTemp ++;
    u32LzmaBufAddr = u32LzmaBufAddr | ((*pTemp) << 24);
    pTemp ++;
    u32LzmaBufAddr = u32LzmaBufAddr | ((*pTemp) << 16);
    pTemp++;
    u32LzmaBufAddr = u32LzmaBufAddr | ((*pTemp) << 8);
    pTemp++;
    u32LzmaBufAddr = u32LzmaBufAddr | ((*pTemp));

    u32LzmaDecBufAddr = 0;
    pTemp ++;
    u32LzmaDecBufAddr = u32LzmaDecBufAddr | ((*pTemp) << 24);
    pTemp ++;
    u32LzmaDecBufAddr = u32LzmaDecBufAddr | ((*pTemp) << 16);
    pTemp++;
    u32LzmaDecBufAddr = u32LzmaDecBufAddr | ((*pTemp) << 8);
    pTemp++;
    u32LzmaDecBufAddr = u32LzmaDecBufAddr | ((*pTemp));

    memset(pcmd_buffer, 0 , 256);
    sprintf(pcmd_buffer, "spi_rdc 0x%x 0x%x 0x%x; LzmaDec 0x%x 0x%x 0x%x 0x%x; go 0x%x;",u32LzmaBufAddr,u32AppSPIAddr,appsize,u32LzmaBufAddr,appsize,u32AppDramAddr,u32LzmaDecBufAddr,u32AppRestAddr);

    run_command (pcmd_buffer, 0);

    free(pcmd_buffer);
    return 1;
}
#endif //end ENABLE_HUMMINGBIRD_CUSTOMER
#if ATSC_SDDS_ENABLE
MS_BOOL STB_SDDS_Update(void)
{
    char buffer[256];
    U32 u32OADFrequency = 0; // ex: 570000
    U16 u16OADPID = 0; // 0x150
    u8  u8OADBandwidth = 0; // 1:6MHz, 2:7MHz, 3: 8MHz
    int ret = 0;
    U8 *pTemp;
    U32 u32AppMagicFlag;
    U32 CRC_result=0;
    U32 CRC_value=0;
    U32 addr=0;
    U32 file_size=0;
    U32 u32UbootInfoAddr;
    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
    int progress_cnt = 1;
    #endif

    /*  1.  Get Update info from App store at flash , if no info then continue the boot flow*/
    //Update_Flag = 0;  // temp assign directly

    // Check update or not
    MS_BOOL bGetSDDSInfoFromNVM = FALSE;
    MS_SDDS_PARAM_2_BL msSDDS_info;

    //printf("%s@%d, SizeOf(msSDDS_info)->[%d]. \n", __FUNCTION__, __LINE__, sizeof(msSDDS_info) );
    memset(&msSDDS_info, 0, sizeof(msSDDS_info));
    bGetSDDSInfoFromNVM = MApp_LoadSDDSInfoFromFlash(&msSDDS_info);
    //printf("%s@%d, boolGetSDDSInfoFromNVM[%d]. \n", __FUNCTION__, __LINE__, bGetSDDSInfoFromNVM);

    if(FALSE == bGetSDDSInfoFromNVM)
    {
        printf("%s@%d, Can't read the SDDS info from NVM ! \n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if ( (TRUE != msSDDS_info.bFoundNewSDDS_4BL_Download)||(57000 > msSDDS_info.u32SDDS_Frequency)||(855000 < msSDDS_info.u32SDDS_Frequency)||(0 == msSDDS_info.u8SDDS_Bandwidth)||(0 == msSDDS_info.u16SDDS_PID) )
    {
        printf("%s@%d, Invalid SDDS parameters ! bFoundNewSDDS_4BL_Download[%d], u32SDDS_Frequency[%d], u8SDDS_Bandwidth[%d], u16SDDS_PID[%d]. \n", __FUNCTION__, __LINE__, msSDDS_info.bFoundNewSDDS_4BL_Download, msSDDS_info.u32SDDS_Frequency, msSDDS_info.u8SDDS_Bandwidth, msSDDS_info.u16SDDS_PID);
        return FALSE;
    }

    // Check the update flag.
    pTemp = (U8 *)getenv("OAD_NEED_UPGRADE");
    //printf("%s@%d, SSU upgrade flag[%s], value[%d]. \n", __FUNCTION__, __LINE__, pTemp, pTemp );

    if (NULL == pTemp )
    {
        if (TRUE == msSDDS_info.bFoundNewSDDS_4BL_Download)
        {
            //printf("%s@%d, bFoundNewSDDS_4BL_Download flag[%d]. \n", __FUNCTION__, __LINE__, msSDDS_info.bFoundNewSDDS_4BL_Download );
            setenv("OAD_NEED_UPGRADE", OAD_TRUE);
        }
        else
        {
            //printf("%s@%d, OAD_UPGRADE is NULL. bFoundNewSDDS_4BL_Download flag[%d]. \n", __FUNCTION__, __LINE__, msSDDS_info.bFoundNewSDDS_4BL_Download );
            setenv("OAD_NEED_UPGRADE", OAD_FALSE);
            saveenv();
            return FALSE;
        }
    }
    else
    {
        if (TRUE == msSDDS_info.bFoundNewSDDS_4BL_Download)
        {
            setenv("OAD_NEED_UPGRADE", OAD_TRUE);
            //printf("%s@%d, SaveSDDSInfoIntoFlash(), bFoundNewSDDS_4BL_Download[%d], u32SDDS_Frequency[%d], u8SDDS_Bandwidth[%d], u16SDDS_PID[%d]. \n", __FUNCTION__, __LINE__, msSDDS_info.bFoundNewSDDS_4BL_Download, msSDDS_info.u32SDDS_Frequency, msSDDS_info.u8SDDS_Bandwidth, msSDDS_info.u16SDDS_PID);
        }
        else
        {
            //printf("%s@%d, SaveSDDSInfoIntoFlash(), bFoundNewSDDS_4BL_Download[%d], u32SDDS_Frequency[%d], u8SDDS_Bandwidth[%d], u16SDDS_PID[%d]. \n", __FUNCTION__, __LINE__, msSDDS_info.bFoundNewSDDS_4BL_Download, msSDDS_info.u32SDDS_Frequency, msSDDS_info.u8SDDS_Bandwidth, msSDDS_info.u16SDDS_PID);
            //setenv("OAD_NEED_UPGRADE", OAD_FALSE);
            //saveenv();    //infinite auto reboot without attaching USB device.
            return FALSE;
        }

    }

    //printf("%s@%d, bFoundNewSDDS_4BL_Download[%d], u32SDDS_Frequency[%d], u8SDDS_Bandwidth[%d], u16SDDS_PID[%d]. \n", __FUNCTION__, __LINE__, msSDDS_info.bFoundNewSDDS_4BL_Download, msSDDS_info.u32SDDS_Frequency, msSDDS_info.u8SDDS_Bandwidth, msSDDS_info.u16SDDS_PID);
    u32OADFrequency = msSDDS_info.u32SDDS_Frequency;    // ex: 570000
    u16OADPID = msSDDS_info.u16SDDS_PID;                // 0x150
    u8OADBandwidth = msSDDS_info.u8SDDS_Bandwidth;      // 1:6MHz, 2:7MHz, 3: 8MHz
    if ( (0 >= u8OADBandwidth)||(3 < u8OADBandwidth) )
    {
        u8OADBandwidth = 1;     // default bandwidth, 1:6MHz..
    }

    //printf("OK\n");
    //printf("OAD Option Freq = %08ld , Pid = %08lX , Band = %08ld \n",u32OADFrequency,u16OADPID,u8OADBandwidth);
#if WDT_ENABLE_HB
    MDrv_WDT_Stop(E_WDT_DBGLV_INFO);
#endif

    /*  2.  Run OAD command and download the app to UPGRADE_BUFFER_ADDR*/
    sprintf(buffer, "oad %d %d %x", u32OADFrequency, u8OADBandwidth,u16OADPID);
    ret = run_command(buffer, 0);

    if(ret != 0 )
    {
        MS_BOOL bWriteSDDSBackOK = FALSE;

        printf("cmd failed %s \n", buffer);
        msSDDS_info.bFoundNewSDDS_4BL_Download = FALSE;
        bWriteSDDSBackOK = MApp_SaveSDDSInfoIntoFlash(&msSDDS_info);
        //printf("%s@%d, SaveSDDSInfoIntoFlash(), bFoundNewSDDS_4BL_Download[%d], u32SDDS_Frequency[%d], u8SDDS_Bandwidth[%d], u16SDDS_PID[%d]. \n", __FUNCTION__, __LINE__, msSDDS_info.bFoundNewSDDS_4BL_Download, msSDDS_info.u32SDDS_Frequency, msSDDS_info.u8SDDS_Bandwidth, msSDDS_info.u16SDDS_PID);
        goto ErrorExit;
    }

    /*  3.  Wrtie back to the flash and clear the update info then reboot */
    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        if (progress_cnt < 100)
        {
            progress_cnt += 20;
            sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 5, GRAPHIC_WIDTH, LINE_HEIGHT);
            run_command(buffer, 0);
            sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
            run_command(buffer, 0);
        }
    #endif

        pTemp = (U8 *)((U32)UPGRADE_BUFFER_ADDR);
        pTemp = pTemp + STB_UBOOT_LOGO_ROM_END_ADDR;

        // u32UbootInfoAddr
        u32UbootInfoAddr = 0;
        u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp));
        pTemp ++;
        u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp) << 8);
        pTemp++;
        u32UbootInfoAddr = u32UbootInfoAddr | (((*pTemp) & (0xF)) << 16);

        if(u32UbootInfoAddr == 0)
        {
            pTemp = (U8 *)((U32)UPGRADE_BUFFER_ADDR);
            pTemp = pTemp + STB_UBOOT_ROM_END_ADDR;

            // u32UbootInfoAddr
            u32UbootInfoAddr = 0;
            u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp));
            pTemp ++;
            u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp) << 8);
            pTemp++;
            u32UbootInfoAddr = u32UbootInfoAddr | (((*pTemp) & (0xF)) << 16);
        }

        printf("u32UbootInfoAddr: 0x%lx\n", u32UbootInfoAddr);

        u32UbootInfoAddr = u32UbootInfoAddr + 65535;
        // To get the first APP magic flag which is at uboot_size(align to 0x10000) + STB_UBOOT_INFO_ADDR(sboot size)
        u32UbootInfoAddr = ((u32UbootInfoAddr >> 16) << 16) + STB_UBOOT_INFO_ADDR;

        printf("STB_INFO_ADDR: 0x%lx\n", u32UbootInfoAddr);

        pTemp = (U8 *)((U32)UPGRADE_BUFFER_ADDR);
        //pTemp = pTemp + STB_INFO_ADDR;
        pTemp = pTemp + u32UbootInfoAddr;

        // MagicFlag
        u32AppMagicFlag = 0;
        u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 24);
        pTemp ++;
        u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 16);
        pTemp++;
        u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 8);
        pTemp++;
        u32AppMagicFlag = u32AppMagicFlag | ((*pTemp));

        if(u32AppMagicFlag != 0xDEADBEEF)
        {
            printf("AppMagicFlag ERROR !!\n");
            //return FALSE;
            goto ErrorExit;
        }
        else
        {
            printf("AppMagicFlag is correct !!\n");
        }

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        if (progress_cnt < 100)
        {
            progress_cnt += 20;
            sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 5, GRAPHIC_WIDTH, LINE_HEIGHT);
            run_command(buffer, 0);
            sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
            run_command(buffer, 0);
        }
    #endif

        file_size = simple_strtol(getenv("loadsize"), NULL, 16);
        printf("Check File Size 0x%lx. \n", file_size);
        if((file_size <= 4) || (file_size%4 != 0))
        //if(file_size <= 4)
        {
            //wrong file size
            printf("wrong file size: 0x%lx\n", file_size);
            //return FALSE;
            goto ErrorExit;
        }

        addr = UPGRADE_BUFFER_ADDR+(file_size-4);
        CRC_value = CRC_result+1;
        if(addr > UPGRADE_BUFFER_ADDR)
        {
            CRC_value=*((U32*)(addr));
        }
        CRC_result=crc32( 0x00, (U8*)UPGRADE_BUFFER_ADDR, file_size-4 );
        if(CRC_result!= CRC_value)
        {
            //CRC error
            printf("CRC check error !!\n");
            //return FALSE;
            goto ErrorExit;
        }
        else
        {
            printf("CRC check success !!\n");
        }

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        if (progress_cnt < 100)
        {
            progress_cnt += 20;
            sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 5, GRAPHIC_WIDTH, LINE_HEIGHT);
            run_command(buffer, 0);
            sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
            run_command(buffer, 0);
        }
    #endif

        memset(buffer, 0 , 256);
        sprintf(buffer, "%lX", file_size);
        setenv("loadsize", buffer);

        file_size = file_size + 65535;
        file_size = ((file_size >> 16)<<16);
        memset(buffer, 0 , 256);
        sprintf(buffer,"spi_wrc 0x%x 0x80000 0x%x", UPGRADE_BUFFER_ADDR+0x80000, file_size-0x80000);
        printf("%s, file size[0x%x].\n", buffer, file_size);
        printf("0\n");

        ret = run_command(buffer, 0);

        if(ret!=0)
        {
            printf("cmd %s failed\n", buffer);
            //return FALSE;
            goto ErrorExit;
        }
        else
        {
            printf("cmd %s success\n", buffer);
        }

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        progress_cnt = 100;
        sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
        run_command(buffer, 0);
        sprintf(buffer, "draw_string %d %d 0x3fffffff 1 SOFTWARE UPGRADE COMPLETE", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
        run_command(buffer, 0);
        sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
        run_command(buffer, 0);
        udelay(1000000);
        sprintf(buffer, "osd_destroy"); //clear screen
        run_command(buffer, 0);

    #endif

        if (TRUE == msSDDS_info.bFoundNewSDDS_4BL_Download)
        {
            MS_BOOL bWriteSDDSBackOK = FALSE;

            setenv("OAD_NEED_UPGRADE", OAD_FALSE);
            msSDDS_info.bFoundNewSDDS_4BL_Download = FALSE;
            bWriteSDDSBackOK = MApp_SaveSDDSInfoIntoFlash(&msSDDS_info);
            //printf("%s@%d, SaveSDDSInfoIntoFlash(), bFoundNewSDDS_4BL_Download[%d], u32SDDS_Frequency[%d], u8SDDS_Bandwidth[%d], u16SDDS_PID[%d]. \n", __FUNCTION__, __LINE__, msSDDS_info.bFoundNewSDDS_4BL_Download, msSDDS_info.u32SDDS_Frequency, msSDDS_info.u8SDDS_Bandwidth, msSDDS_info.u16SDDS_PID);
        }
        else
        {
            setenv("OAD_NEED_UPGRADE", OAD_FALSE);
        }

        setenv("oad_complete", OAD_DONE);
        //saveenv();    // system crash if writing the wrong flash address.
        return TRUE;

    ErrorExit:

        printf("============== OAD_UPDATE FAIL !!! =============== \n");
        setenv("OAD_NEED_UPGRADE",OAD_NONE);
        setenv("oad_complete", OAD_FAIL);
        saveenv();

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
        run_command(buffer, 0);
        sprintf(buffer, "draw_string %d %d 0x3fffffff 1 SOFTWARE UPGRADE ERROR", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
        run_command(buffer, 0);
        udelay(5000000);
        sprintf(buffer, "reset");
        run_command(buffer, 0);
    #endif

        return FALSE;
}

static MS_BOOL MApp_LoadSDDSInfoFromFlash(MS_SDDS_PARAM_2_BL *SDDSinfo)
{
    MS_BOOL bStatus = FALSE;

    // Read SDDS information from flash.
    bStatus = MDrv_SERFLASH_Read(BASEADDRESS_SDDS_INFO_IN_FLASH , MemAlign(sizeof(MS_SDDS_PARAM_2_BL), 4), (MS_U8 *)SDDSinfo);
	/*
    printf(" MApp_LoadSDDSInfoFromFlash  u16SDDS_PID = %d u32Freq = %d flag = %u \n",SDDSinfo->u16SDDS_PID,SDDSinfo->u32SDDS_Frequency,SDDSinfo->bFoundNewSDDS_4BL_Download);
    printf(" MApp_LoadSDDSInfoFromFlash SW = %u, HW = %u, SchCount = %u, SchTime = %lu  Timeleng = %lu \n",SDDSinfo->u16SDDS_SW_Verion,SDDSinfo->u16SDDS_HW_Verion,SDDSinfo->u8ScheduleDescCount,SDDSinfo->u32SchStartTime,SDDSinfo->u32SchLengthInSeconds);
    printf(" MApp_LoadSDDSInfoFromFlash SwMID = %u, HwMID = %u, OUI = %u \n",SDDSinfo->u16SDDS_SW_ModelID,SDDSinfo->u16SDDS_HW_ModelID,SDDSinfo->u32SDDS_OUI);
	*/
    return bStatus;
}

static MS_BOOL MApp_SaveSDDSInfoIntoFlash(MS_SDDS_PARAM_2_BL *_SDDSinfo)
{
    MS_BOOL bStatus = FALSE;

    //erase flash
    if (TRUE == MDrv_SERFLASH_WriteProtect(FALSE) )
    {
        if (TRUE == MDrv_SERFLASH_AddressErase(BASEADDRESS_SDDS_INFO_IN_FLASH, MemAlign(sizeof(MS_SDDS_PARAM_2_BL), 4) , TRUE))
        {
            //write database into flash
            if (TRUE == MDrv_SERFLASH_Write(BASEADDRESS_SDDS_INFO_IN_FLASH, MemAlign(sizeof(MS_SDDS_PARAM_2_BL), 4), (MS_U8 *)&_SDDSinfo ))
            {
                bStatus = TRUE;
            }
            else
            {
                bStatus = FALSE;
                printf("%s@%d, SERFLASH_write fail !\n", __FUNCTION__, __LINE__ );
            }
        }
        else
        {
            printf("%s@%d, SERFLASH_AddressErase fail !\n", __FUNCTION__, __LINE__ );
        }
    }
    else
    {
        printf("%s@%d, SERFLASH_WriteProtect fail !\n", __FUNCTION__, __LINE__ );
    }

    return bStatus;
}
#else

#if (ENABLE_HUMMINGBIRD_CUSTOMER)
static char *get_script_next_line(char **line_buf_ptr);
#if ENABLE_DVB_S_SYS

MS_BOOL MApi_LDR_GetOADUpateInfo(void)
{
    char *pTemp;
    MS_SAT_PARAM stSATParam;
    MS_FE_CARRIER_PARAM stTunerParam;

    pTemp = getenv("OAD_NEED_UPGRADE");
    if (!pTemp)
    {
        return FALSE;
    }
    if (simple_strtoul(pTemp, NULL, 10) != 1)
    {
        return FALSE;
    }

    memset(&stSATParam, 0, sizeof(MS_SAT_PARAM));
    memset(&stTunerParam, 0, sizeof(MS_FE_CARRIER_PARAM));

    pTemp = getenv("OAD_u16LoLOF");
    if (pTemp == NULL)
    {
        return FALSE;
    }
    stSATParam.u16LoLOF = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_u16LoLOF = %d\n", stSATParam.u16LoLOF);

    pTemp = getenv("OAD_u16HiLOF");
    if (pTemp == NULL)
    {
        return FALSE;
    }
    stSATParam.u16HiLOF = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_u16HiLOF = %d\n", stSATParam.u16HiLOF);

    pTemp = getenv("OAD_eLNBType");
    if (pTemp == NULL)
    {
        return FALSE;
    }
    stSATParam.eLNBType = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_eLNBType = %d\n", stSATParam.eLNBType);

    pTemp = getenv("OAD_eDiseqcLevel");
    if (pTemp == NULL)
    {
        return FALSE;
    }
    stSATParam.eDiseqcLevel = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_eDiseqcLevel = %d\n", stSATParam.eDiseqcLevel);

    pTemp = getenv("OAD_eToneburstType");
    if (pTemp == NULL)
    {
        return FALSE;
    }
    stSATParam.eToneburstType = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_eToneburstType = %d\n", stSATParam.eToneburstType);

    pTemp = getenv("OAD_eSwt10Port");
    if (pTemp == NULL)
    {
        return FALSE;
	}
    stSATParam.eSwt10Port = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_eSwt10Port = %d\n", stSATParam.eSwt10Port);


    pTemp = getenv("OAD_eSwt11Port");
    if (pTemp == NULL)
    {
        return FALSE;
	}
    stSATParam.eSwt11Port = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_eSwt11Port = %d\n", stSATParam.eSwt11Port);

    pTemp = getenv("OAD_e22KOnOff");
    if (pTemp == NULL)
    {
        return FALSE;
	}
    stSATParam.e22KOnOff = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_e22KOnOff = %d\n", stSATParam.e22KOnOff);

    pTemp = getenv("OAD_eLNBPwrOnOff");
    if (pTemp == NULL)
    {
        return FALSE;
    }
    stSATParam.eLNBPwrOnOff = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_eLNBPwrOnOff = %d\n", stSATParam.eLNBPwrOnOff);


    pTemp = getenv("OAD_e0V12VOnOff");
    if (pTemp == NULL)
    {
        return FALSE;
	}
    stSATParam.e0V12VOnOff = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_e0V12VOnOff = %d\n", stSATParam.e0V12VOnOff);

    pTemp = getenv("OAD_u8Position");
    if (pTemp == NULL)
    {
        return FALSE;
	}
    stSATParam.u8Position = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_u8Position = %d\n", stSATParam.u8Position);

    pTemp = getenv("OAD_u16Angle");
    if (pTemp == NULL)
    {
        return FALSE;
	}
    stSATParam.u16Angle = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_u16Angle = %d\n", stSATParam.u16Angle);

    //carrier info
    pTemp = getenv("OAD_Fre");
    if (pTemp == NULL)
    {
        return FALSE;
	}
    stTunerParam.u32Frequency = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_Fre = %d\n", stTunerParam.u32Frequency);

    pTemp = getenv("OAD_Sym");
    if (pTemp == NULL)
    {
        return FALSE;
	}
    stTunerParam.SatParam.u32SymbolRate = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_Sym = %d\n", stTunerParam.SatParam.u32SymbolRate);

    pTemp = getenv("OAD_Pol");
    if (pTemp == NULL)
    {
        return FALSE;
	}
    stTunerParam.SatParam.u8Polarity = simple_strtoul(pTemp, NULL, 10);
    printf("OAD_Pol = %d\n", stTunerParam.SatParam.u8Polarity);

    MApi_FE_SetSATParam(&stSATParam);
    MApi_FE_SetFECarrierParam(&stTunerParam);
    return TRUE;
}

#endif

#endif//end ENABLE_HUMMINGBIRD_CUSTOMER
MS_BOOL STB_OAD_Update(void)
{
    char buffer[256];
    U32 u32OADFrequency = 0; // ex: 570000
    U16 u16OADPID = 0; // 0x150
    u8  u8OADBandwidth = 0; // 1:6MHz, 2:7MHz, 3: 8MHz
    int ret = 0;
    U8 *pTemp;
	
#if (!ENABLE_HUMMINGBIRD_CUSTOMER)
    U32 u32AppMagicFlag;
#endif
    U32 CRC_result=0;
    U32 CRC_value=0;
    U32 addr=0;
    U32 file_size=0;

    #if (!ENABLE_HUMMINGBIRD_CUSTOMER)
    U32 u32UbootInfoAddr;
	#endif

	#if (ENABLE_HUMMINGBIRD_CUSTOMER)
    char *script_buf;
    char *next_line;
	#endif

	#if (!ENABLE_HUMMINGBIRD_CUSTOMER)
    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
    int progress_cnt = 1;
    #endif
	#endif
    /*  1.  Get Update info from App store at flash , if no info then continue the boot flow*/
    //Update_Flag = 0;  // temp assign directly

    // check MBOOT Version

    pTemp = (U8 *)getenv("OAD_IN_MBOOT") ;
    if(NULL == pTemp) return FALSE ;

    // Check update or not
    pTemp = (U8 *)getenv("OAD_NEED_UPGRADE");
    if (NULL == pTemp )  return FALSE ;
    if (0!=strcmp((char*)pTemp , OAD_TRUE)) return FALSE ;


    // Check update status
    pTemp = (U8 *)getenv("oad_complete");
    if (NULL == pTemp )  return FALSE ;
    if (0 == strcmp((char*)pTemp,OAD_DONE) )
    {
        setenv("OAD_NEED_UPGRADE",OAD_FALSE);
        saveenv();
        return FALSE;
    }

    // check OAD parameter
    pTemp = (U8 *)getenv("OAD_FREQ") ;
    if (NULL == pTemp) return FALSE ;
    u32OADFrequency = simple_strtol((const char*)pTemp, NULL, 10);

    pTemp = (U8 *)getenv("OAD_BAND") ;
    if (NULL == pTemp) return FALSE ;
    u8OADBandwidth  = simple_strtol((const char*)pTemp, NULL, 10);

    pTemp = (U8 *)getenv("OAD_PID") ;
    if (NULL == pTemp) return FALSE ;
    u16OADPID = (U16)simple_strtol((const char*)pTemp, NULL, 10);

    printf("OK\n");
    printf("OAD Option Freq = %08ld , Pid = %08lX , Band = %08ld \n",u32OADFrequency,u16OADPID,u8OADBandwidth);
#if WDT_ENABLE_HB
    MDrv_WDT_Stop(E_WDT_DBGLV_INFO);
#endif
#if (ENABLE_HUMMINGBIRD_CUSTOMER)

#if ENABLE_DVB_S_SYS
MApi_LDR_GetOADUpateInfo();
#endif

#endif

    /*  2.  Run OAD command and download the app to UPGRADE_BUFFER_ADDR*/
    sprintf(buffer, "oad %d %d %x", u32OADFrequency, u8OADBandwidth,u16OADPID);
    ret = run_command(buffer, 0);

    if(ret != 0 )
    {
         printf("cmd failed %s \n",buffer);
         goto ErrorExit;
    }

    /*  3.  Wrtie back to the flash and clear the update info then reboot */
#if (!ENABLE_HUMMINGBIRD_CUSTOMER)
    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        if (progress_cnt < 100)
        {
            progress_cnt += 20;
            sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 5, GRAPHIC_WIDTH, LINE_HEIGHT);
            run_command(buffer, 0);
            sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
            run_command(buffer, 0);
        }
    #endif

        pTemp = (U8 *)((U32)UPGRADE_BUFFER_ADDR);
        pTemp = pTemp + STB_UBOOT_LOGO_ROM_END_ADDR;

        // u32UbootInfoAddr
        u32UbootInfoAddr = 0;
        u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp));
        pTemp ++;
        u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp) << 8);
        pTemp++;
        u32UbootInfoAddr = u32UbootInfoAddr | (((*pTemp) & (0xF)) << 16);

        if(u32UbootInfoAddr == 0)
        {
            pTemp = (U8 *)((U32)UPGRADE_BUFFER_ADDR);
            pTemp = pTemp + STB_UBOOT_ROM_END_ADDR;

            // u32UbootInfoAddr
            u32UbootInfoAddr = 0;
            u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp));
            pTemp ++;
            u32UbootInfoAddr = u32UbootInfoAddr | ((*pTemp) << 8);
            pTemp++;
            u32UbootInfoAddr = u32UbootInfoAddr | (((*pTemp) & (0xF)) << 16);
        }

        printf("u32UbootInfoAddr: 0x%lx\n", u32UbootInfoAddr);

        u32UbootInfoAddr = u32UbootInfoAddr + 65535;
        // To get the first APP magic flag which is at uboot_size(align to 0x10000) + STB_UBOOT_INFO_ADDR(sboot size)
        u32UbootInfoAddr = ((u32UbootInfoAddr >> 16) << 16) + STB_UBOOT_INFO_ADDR;

        printf("STB_INFO_ADDR: 0x%lx\n", u32UbootInfoAddr);

        pTemp = (U8 *)((U32)UPGRADE_BUFFER_ADDR);
        //pTemp = pTemp + STB_INFO_ADDR;
        pTemp = pTemp + u32UbootInfoAddr;

        // MagicFlag
        u32AppMagicFlag = 0;
        u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 24);
        pTemp ++;
        u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 16);
        pTemp++;
        u32AppMagicFlag = u32AppMagicFlag | ((*pTemp) << 8);
        pTemp++;
        u32AppMagicFlag = u32AppMagicFlag | ((*pTemp));


        if(u32AppMagicFlag != 0xDEADBEEF)
        {
            printf("AppMagicFlag ERROR !!\n");
            //return FALSE;
            goto ErrorExit;
        }
        else
        {
            printf("AppMagicFlag is correct !!\n");
        }

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        if (progress_cnt < 100)
        {
            progress_cnt += 20;
            sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 5, GRAPHIC_WIDTH, LINE_HEIGHT);
            run_command(buffer, 0);
            sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
            run_command(buffer, 0);
        }
    #endif

        file_size = simple_strtol(getenv("loadsize"), NULL, 16);
        printf("Check File Size %.x \n",file_size);
        if((file_size <= 4) || (file_size%4 != 0))
        //if(file_size <= 4)
        {
            //wrong file size
            printf("wrong file size: 0x%lx\n", file_size);
            //return FALSE;
            goto ErrorExit;
        }

        addr = UPGRADE_BUFFER_ADDR+(file_size-4);
        CRC_value = CRC_result+1;
        if(addr > UPGRADE_BUFFER_ADDR)
        {
            CRC_value=*((U32*)(addr));
        }
        CRC_result=crc32( 0x00, (U8*)UPGRADE_BUFFER_ADDR, file_size-4 );
        if(CRC_result!= CRC_value)
        {
            //CRC error
            printf("CRC check error !!\n");
            //return FALSE;
            goto ErrorExit;
        }
        else
        {
            printf("CRC check success !!\n");
        }

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        if (progress_cnt < 100)
        {
            progress_cnt += 20;
            sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 5, GRAPHIC_WIDTH, LINE_HEIGHT);
            run_command(buffer, 0);
            sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
            run_command(buffer, 0);
        }
    #endif


        memset(buffer, 0 , 256);
        sprintf(buffer, "%lX", file_size);
        setenv("loadsize", buffer);

        file_size = file_size + 65535;
        file_size = ((file_size >> 16)<<16);
        memset(buffer, 0 , 256);
        sprintf(buffer,"spi_wrc 0x%x 0x00000 0x%x", UPGRADE_BUFFER_ADDR,file_size);
        printf("%s\n",buffer);
        printf("0\n");

        ret = run_command(buffer, 0);

        if(ret!=0)
        {
            printf("cmd %s failed\n", buffer);
            //return FALSE;
            goto ErrorExit;
        }
        else
        {
            printf("cmd %s success\n", buffer);
        }

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        progress_cnt = 100;
        sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
        run_command(buffer, 0);
        sprintf(buffer, "draw_string %d %d 0x3fffffff 1 SOFTWARE UPGRADE COMPLETE", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
        run_command(buffer, 0);
        sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
        run_command(buffer, 0);
        udelay(1000000);
        sprintf(buffer, "osd_destroy"); //clear screen
        run_command(buffer, 0);

    #endif
#else //else ENABLE_HUMMINGBIRD_CUSTOMER



        file_size = simple_strtol(getenv("loadsize"), NULL, 16);
        printf("Check File Size %.x \n",file_size);
        if((file_size <= 4) || (file_size%4 != 0))
        //if(file_size <= 4)
        {
            //wrong file size
            printf("wrong file size: 0x%lx\n", file_size);
            //return FALSE;
            goto ErrorExit;
        }

        addr = UPGRADE_BUFFER_ADDR+(file_size-4);
        CRC_value = CRC_result+1;
        if(addr > UPGRADE_BUFFER_ADDR)
        {
            CRC_value=*((U32*)(addr));
        }
        CRC_result=crc32( 0x00, (U8*)UPGRADE_BUFFER_ADDR, file_size-4 );
        if(CRC_result!= CRC_value)
        {
            //CRC error
            printf("CRC check error !!\n");
            //return FALSE;
            goto ErrorExit;
        }
        else
        {
            printf("CRC check success !!\n");
        }




script_buf = (char *)UPGRADE_BUFFER_ADDR ;

while ((next_line = get_script_next_line(&script_buf)) != NULL)
{
            printf("\n>> %s \n", next_line);



            ret = run_command(next_line, 0);
            if (ret < 0)
            {
                printf("Error> cmd: %s FAIL !!\n", next_line);
                break;
            }

  }
#endif //!ENABLE_HUMMINGBIRD_CUSTOMER

        setenv("oad_complete", OAD_DONE);
        saveenv();
        return TRUE;

    ErrorExit:

        printf("============== OAD_UPDATE FAIL !!! =============== \n");
        setenv("OAD_NEED_UPGRADE",OAD_NONE);
        setenv("oad_complete", OAD_FAIL);
        saveenv();

    #if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
        run_command(buffer, 0);
        sprintf(buffer, "draw_string %d %d 0x3fffffff 1 SOFTWARE UPGRADE ERROR", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
        run_command(buffer, 0);
        udelay(5000000);
        sprintf(buffer, "reset");
        run_command(buffer, 0);

    #endif

        return FALSE;


}
#endif  //end of ATSC_SDDS_ENABLE
#endif
#endif


#define TSPCLK_MAP  0xBF2016A8
#define TSP      ((unsigned char volatile *) TSPCLK_MAP)

#define MIU_MAP  0xBF20254c
#define MIU      ((unsigned char volatile *) MIU_MAP)

U8 u8ir_pressed = 0;
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
MS_BOOL MApi_LDR_GetOADBasicInfo(void);
#endif
void auto_boot(void)
{
#if defined(CONFIG_USB_AUTO_UPDATE)||defined (CONFIG_OAD_IN_MBOOT)
    int ret=0;
    int iUsbPort;
    char buffer[128];
#endif
#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
	char *s;
#endif

#if (CONFIG_COMMANDS & CFG_CMD_SECURE)
                Secure_Application_Authentication();
#endif


#if defined(CONFIG_KRONUS)||defined(CONFIG_URANUS4) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA) || defined(CONFIG_KELTIC)|| defined(CONFIG_KRITI)|| defined(CONFIG_KERES)

#if WDT_ENABLE_HB
    printf("WDT Initialize ... \n");
    MDrv_WDT_Init(E_WDT_DBGLV_INFO);
    MDrv_WDT_SetTimer(E_WDT_DBGLV_INFO,8);
#endif


#if (CONFIG_USB)

#if (ENABLE_HUMMINGBIRD_CUSTOMER)

MApi_LDR_GetOADBasicInfo();

#endif

#if defined (CONFIG_USB_STORAGE)

    check_usb_upgrade();  //fix fatload bugs
#endif

#if defined(CONFIG_USB_AUTO_UPDATE)

#if defined(CONFIG_KELTIC) || defined(CONFIG_KERES)
    int polling_port_max = 1;;
#else
    int polling_port_max = 3;;
#endif

    for(iUsbPort = 0 ;iUsbPort < polling_port_max; iUsbPort++)
    {
        if(usb_init(iUsbPort) == 0)
        {
            if (usb_stor_scan(1) == 0)
            {
                ret = 1 ;
                //ClearValue = iUsbPort;
                break;
            }
        }
    }

// device partition to proper one
    block_dev_desc_t *dev_desc=NULL;

    extern block_dev_desc_t *get_dev (char* ifname, int dev);
    dev_desc = get_dev("usb",0);
    if (dev_desc != NULL)
    {
        int ret = 0;
        int i=0;
        for(i=1;i<=4;i++)
        {
            if (fat_register_device(dev_desc,i) == 0)
            {
                if (i==1)
                    break;
                printf("partition %d\n",i);
                sprintf(buffer,"setenv partno %d",i);
                ret = run_command(buffer, 0);
                break;
            }
        }
    }
// end

    if (ret)
    {
        memset(buffer, 0 , 128);
        sprintf(buffer,"fatls usb 0");
        run_command(buffer, 0);
    }

    //printf("%s@%d, USB_UpdateFlag[%s]. \n", __FUNCTION__, __LINE__, getenv("USBUpdateFlag") );
    if((getenv("USBUpdateFlag")!= NULL)&& (ret == 1)) //
    {
#if WDT_ENABLE_HB
        MDrv_WDT_Stop(E_WDT_DBGLV_INFO);
#endif
        printf("USB Update flag  %d \n", simple_strtol(getenv("USBUpdateFlag"), NULL, 16));
        if(simple_strtol(getenv("USBUpdateFlag"), NULL, 16)==1)//check  Enable USB update flag    simple_strtol(getenv("USBUpdateFlag"), NULL, 16) == 1)
        {
            ret = STB_USB_AutoUpdate();
            //printf("%s@%d, STB_USB_autoUpdateFlag[%d]. \n", __FUNCTION__, __LINE__, ret);
            if(ret == FALSE)
            {
                memset(buffer, 0 , 128);
                sprintf(buffer,"setenv USBUpdateFail %d", 1);
                ret = run_command(buffer, 0);

                if(ret!=0)
                {
                    printf("cmd %s failed\n", buffer);
                }
                else
                {
                    printf("cmd %s success\n", buffer);
                }

                memset(buffer, 0 , 128);
                sprintf(buffer,"setenv USBUpdateFlag 0"); // set usb update to disable
                ret = run_command(buffer, 0);

                if(ret!=0)
                {
                    printf("cmd %s failed\n", buffer);
                }
                else
                {
                    printf("cmd %s success\n", buffer);
                }
            }
            else
            {
                //printf("%s@%d, STB_USB_AutoUpdate return true. \n", __FUNCTION__, __LINE__);
                ret = STB_Read_Boot_info();
                if(ret == FALSE)
                {
                    memset(buffer, 0 , 128);
                    sprintf(buffer,"setenv USBUpdateFail %d",1);
                    ret = run_command(buffer, 0);

                    if(ret!=0)
                    {
                        printf("cmd %s failed\n", buffer);
                    }
                    else
                    {
                        printf("cmd %s success\n", buffer);
                    }

                    memset(buffer, 0 , 128);
                    sprintf(buffer,"setenv USBUpdateFlag %d", 1);
                    ret = run_command(buffer, 0);

                    if(ret!=0)
                    {
                        printf("cmd %s failed\n", buffer);
                    }
                    else
                    {
                        printf("cmd %s success\n", buffer);
                    }

                }
                else
                {
                    memset(buffer, 0 , 128);
                    sprintf(buffer,"setenv USBUpdateFail %d",0);
                    ret = run_command(buffer, 0);

                    if(ret!=0)
                    {
                        printf("cmd %s failed\n", buffer);
                    }
                    else
                    {
                        printf("cmd %s success\n", buffer);
                    }

                    memset(buffer, 0 , 128);
                    sprintf(buffer,"setenv USBUpdateFlag %d",0);
                    ret = run_command(buffer, 0);

                    if(ret!=0)
                    {
                        printf("cmd %s failed\n", buffer);
                    }
                    else
                    {
                        printf("cmd %s success\n", buffer);
                    }
                }
            }
            memset(buffer, 0 , 128);
            sprintf(buffer, "saveenv");
            ret = run_command (buffer, 0);

            if(ret!=0)
            {
                printf("cmd %s failed\n", buffer);
            }
            else
            {
                printf("cmd %s success\n", buffer);
            }

            printf("[%s][%d]RESET!! \n", __FUNCTION__, __LINE__);
            sprintf(buffer, "reset");
            run_command(buffer, 0);
        }
     }
    else
 #endif
    {
        if(getenv("bootcmd") == NULL)
        {
            //printf("%s@%d, BootCmd is null. \n", __FUNCTION__, __LINE__);
            STB_Read_Boot_info();// direct boot from SPI without update. when null pointer
        }
    }


#endif

#if (CONFIG_OAD & CONFIG_OAD_IN_MBOOT)

#if ATSC_SDDS_ENABLE
    if(STB_SDDS_Update()!= FALSE)
#else
    if(STB_OAD_Update()!= FALSE)
#endif
    {
        //printf("%s@%d, doesn't SSU update. \n", __FUNCTION__, __LINE__);
        STB_Read_Boot_info();

        printf("[%s][%d]RESET!! \n", __FUNCTION__, __LINE__);
        //udelay(2000000);
        sprintf(buffer, "reset");
        run_command(buffer, 0);
    }
    else
    {
        if(getenv("bootcmd") == NULL)
        {
            printf("%s@%d, BootCmd is null. \n", __FUNCTION__, __LINE__);
            STB_Read_Boot_info();// direct boot from SPI without update. when null pointer
        }
    }
#endif
    printf("[%s][%d], No immediate SW RESET. \n", __FUNCTION__, __LINE__);

#endif

#if !defined(CONFIG_JANUS) && !defined(CONFIG_MARIA10)
            {
        #include <sys/common/MsTypes.h>
        #include <sys/common/MsDevice.h>
        #include <sys/common/c_riubase.h>
        #include <sys/common/MsIRQ.h>
              //  #include <sys/common/MsOS.h>
        #include <sys/drv/drvMIU.h>
                // Added by coverity_0210d
#if defined(CONFIG_KRONUS)||defined(CONFIG_KAISERIN)||defined(CONFIG_KELTIC)||defined(CONFIG_KERES)
                U8 zero_array[8] = {0, 0, 0, 0, 0, 0, 0, 0};
#else
                U8 zero_array[4] = {0, 0, 0, 0};
#endif
                U32 u32TmpStart, u32TmpEnd;
#if defined(CONFIG_KAISERIN)|| defined(CONFIG_KELTIC)||defined(CONFIG_KERES)
                U32 u32Size,pageshift;
#endif
                extern U8  _readonly_start[];
                extern U8  _readonly_end[];
                printf("Start %x End %x \n",MsOS_VA2PA((U32)_readonly_start),MsOS_VA2PA((U32)_readonly_end) );

                // Modified by coverity_0210
                u32TmpStart = MsOS_VA2PA((U32)_readonly_start);
                u32TmpEnd = MsOS_VA2PA((U32)_readonly_end);
#if (!CONFIG_MSTAR_SILENT)
                printf("UnProtect MIU block 0 : 0x%x 0x%x\n", u32TmpStart, u32TmpEnd);

#endif

#if defined(CONFIG_KAISERIN)||defined(CONFIG_KELTIC)||defined(CONFIG_KERES)
                u32Size = u32TmpEnd - u32TmpStart;
                MDrv_MIU_ProtectAlign((MS_U32 *)&pageshift);
                MDrv_MIU_Protect(0, zero_array, ALIGN(u32TmpStart, pageshift), ALIGN(u32TmpStart + u32Size -1, pageshift), DISABLE);
#else
                MDrv_MIU_Protect(0, zero_array, u32TmpStart, u32TmpEnd, DISABLE);
#endif
            }
#endif





#if (CONFIG_SILENT_CONSOLE)
        gd->flags &= ~GD_FLG_SILENT;
#if (!CONFIG_MSTAR_SILENT)
	    puts("Disable Silent Mode\n");
#endif
#endif
#if defined (CONFIG_AUTOBOOT_KEYED)
		int prev = disable_ctrlc(1);	/* disable Control C checking */
#endif

        set_miu();
        set_info_exchange_cfg();
        set_env_cfg_2bootargs();
        s = getenv ("bootcmd");

#ifndef CFG_HUSH_PARSER
		run_command (s, 0);
#else
		parse_string_outer(s, FLAG_PARSE_SEMICOLON |
				    FLAG_EXIT_FROM_LOOP);
#endif

#if defined (CONFIG_AUTOBOOT_KEYED)
		disable_ctrlc(prev);	/* restore Control C checking */
#endif
        set_miu4boot();
}


void main_loop (void)
{
    U8 keypad_pressed=0;

#if defined(CONFIG_BOOT_SPI_LINUX)
    char cmd[256];
    memset(cmd, 0 , 256);
    sprintf(cmd, "spi_ptinit");
    run_command (cmd, 0);
#endif

#ifndef CFG_HUSH_PARSER
	static char lastcommand[CFG_CBSIZE] = { 0, };
	int len;
	int rc = 1;
	int flag;
#endif
#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
	char *s;
	int bootdelay;
#endif
#if defined (CONFIG_PREBOOT)
	char *p;
#endif
#if defined (CONFIG_BOOTCOUNT_LIMIT)
	unsigned long bootcount = 0;
	unsigned long bootlimit = 0;
	char *bcs;
	char bcs_set[16];
#endif /* CONFIG_BOOTCOUNT_LIMIT */

	MDrv_BDMA_Init(0);

#if (!CONFIG_MSTAR_SILENT)
	printf ("%s\n", MBOOT_VBuf);
#endif

#if defined (CONFIG_SW_VERSION)
    #if defined (CONFIG_USB_STORAGE)
      if(usb_init(0)>=0)
       {
         udelay(2000*1000);
       }
       run_command("usb start", 0);
    #endif
  #endif

#if (CONFIG_COMMANDS & CFG_CMD_BOOTLOGO)

    char *env_str = getenv("logo_cmd");
    if (env_str == NULL)
    {
#if defined(CONFIG_KELTIC)||defined(CONFIG_KERES)
        setenv("logo_cmd", "boot_logo 8 0 0 0");
#else
        setenv("logo_cmd", "boot_logo 0 0 1 1");
#endif
        env_str = getenv("logo_cmd");
    }
    run_command (env_str, 0);

#endif

#if 1 // for AES DMA
{
    TSP[0]=0x0;
}
#endif

#if defined(CONFIG_VFD) && defined(VFD_TEST_LOGO)
	ulong bmp = 0;		/* default bitmap */
	extern int trab_vfd (ulong bitmap);

#if defined (CONFIG_MODEM_SUPPORT)
	if (do_mdm_init)
		bmp = 1;	/* alternate bitmap */
#endif
	trab_vfd (bmp);
#endif	/* CONFIG_VFD && VFD_TEST_LOGO */

#if defined (CONFIG_BOOTCOUNT_LIMIT)
	bootcount = bootcount_load();
	bootcount++;
	bootcount_store (bootcount);
	sprintf (bcs_set, "%lu", bootcount);
	setenv ("bootcount", bcs_set);
	bcs = getenv ("bootlimit");
	bootlimit = bcs ? simple_strtoul (bcs, NULL, 10) : 0;
#endif /* CONFIG_BOOTCOUNT_LIMIT */

#if defined (CONFIG_MODEM_SUPPORT)
	debug ("DEBUG: main_loop:   do_mdm_init=%d\n", do_mdm_init);
	if (do_mdm_init)
    {
		char *str = strdup(getenv("mdm_cmd"));
		setenv ("preboot", str);  /* set or delete definition */
		if (str != NULL)
			free (str);
		mdm_init(); /* wait for modem connection */
	}
#endif  /* CONFIG_MODEM_SUPPORT */

#if defined (CONFIG_VERSION_VARIABLE)
	{
		extern char version_string[];

		setenv ("ver", version_string);  /* set version variable */
	}
#endif /* CONFIG_VERSION_VARIABLE */

#if defined (CFG_HUSH_PARSER)
	u_boot_hush_start ();
#endif

#if defined (CONFIG_AUTO_COMPLETE)
	install_auto_complete();
#endif

#if defined (CONFIG_PREBOOT)
	if ((p = getenv ("preboot")) != NULL) {
#if defined (CONFIG_AUTOBOOT_KEYED)
		int prev = disable_ctrlc(1);	/* disable Control C checking */
#endif

#if defined (CONFIG_AUTOBOOT_KEYED)
		disable_ctrlc(prev);	/* restore Control C checking */
#endif
	}
#endif /* CONFIG_PREBOOT */

#if (ENABLE_MODULE_JFFS2)
// mtdparts_init is needed on both nand and serial flash platform
    mtdparts_init();
#endif

#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
	s = getenv ("bootdelay");
	bootdelay = s ? (int)simple_strtol(s, NULL, 10) : CONFIG_BOOTDELAY;
	debug ("### main_loop entered: bootdelay=%d\n\n", bootdelay);

#if defined (CONFIG_BOOT_RETRY_TIME)
	init_cmd_timeout ();
#endif	/* CONFIG_BOOT_RETRY_TIME */

#if (CONFIG_SILENT_CONSOLE)
    bootdelay = 0;	//no delay to decrease boot time.
#endif
#if defined (CONFIG_BOOTCOUNT_LIMIT)
	if (bootlimit && (bootcount > bootlimit))
    {
		printf ("Warning: Bootlimit (%u) exceeded. Using altbootcmd.\n", (unsigned)bootlimit);
		s = getenv ("altbootcmd");
	}
#endif /* CONFIG_BOOTCOUNT_LIMIT */

/*******************************************************************************************************/
// add usb auto update function
/*******************************************************************************************************/
#if (CONFIG_COMMANDS & CFG_CMD_EEPROM)
    mdrv_gpio_init();
    MDrv_SwIIC_Init();
#endif
#if defined (CONFIG_WR_FLAG)
    #if(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_TCL)
    {
        printf("start usage have hub!\n");
        upgrade_by_rweeprom_havehub();
    }
    #else
    Upgrade_By_Reeprom();
    #endif
#endif

//use version.txt control usb upgrade
#if defined (CONFIG_SW_VERSION)
    check_sw_version();
#endif

#if defined (CONFIG_KEYPAD)
    printf("keypad_pressed_1 is:%d\n",keypad_pressed);
    config_keypad(&keypad_pressed);
    printf("keypad_pressed_2 is:%d\n",keypad_pressed);
#endif

#if (!CONFIG_MSTAR_SILENT)
printf(" keypad_pressed is [%d] \n", keypad_pressed);
printf(" ir_pressed is [%d] \n", u8ir_pressed);
#endif

#if defined (CONFIG_OAD)
    update_bootarg();
    run_command("ostar", 0); // ostar should run no matter key is pressed or not.
#endif //defined (CONFIG_OAD)

#if defined(CONFIG_DISPLAY_LOGO)
    update_bootlogo_bootarg();
#endif

    if (u8ir_pressed || keypad_pressed)
    {
#if defined (CONFIG_USB_STORAGE)
        int port_switch = -1;
        if (usb_init(0) >= 0)
        {
            port_switch = 0;
        }
        else if (usb_init(1) >= 0)
        {
            port_switch = 1;
        }
        else if (usb_init(2) >= 0)
        {
            port_switch = 2;
        }
        else
        {
            printf("undefined port_switch. \n" );
        }

        if (port_switch != -1)
        {
            printf("port_switch is:%d\n",port_switch);

            if(usb_stor_scan(1)!=-1)
            {
                run_command("ustar",0);
            }
        }
#endif
    }

#if (CONFIG_COMMANDS & CFG_CMD_SECURE)
    Secure_Init();
    Secure_SPI_Init();
    //Secure_GetSSKey();
#if defined (CONFIG_USB_STORAGE)
    if(usb_init(0)>=0)
    {
        if(usb_stor_scan(1)!=-1)
        {
            Secure_USB_KL_Upgrade();
            Secure_USB_CK_Upgrade();
        }
    }
#endif
#endif


    s = getenv ("bootcmd");

	debug ("### main_loop: bootcmd=\"%s\"\n", s ? s : "<UNDEFINED>");

#if defined(CONFIG_KRONUS)||defined(CONFIG_URANUS4) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA) || defined(CONFIG_KELTIC)|| defined(CONFIG_KRITI)|| defined(CONFIG_KERES)
    char temps = 'T';
    if(!s)
    {
        s = &temps;
    }
#endif

    if (bootdelay >= 0 && !abortboot (bootdelay) && s)
    {
        auto_boot();
    }


#if defined (CONFIG_MENUKEY)
	if (menukey == CONFIG_MENUKEY) {
	    s = getenv("menucmd");
	    if (s) {
#ifndef CFG_HUSH_PARSER
		run_command (s, 0);
#else
		parse_string_outer(s, FLAG_PARSE_SEMICOLON |
				    FLAG_EXIT_FROM_LOOP);
#endif
	    }
	}
#endif /* CONFIG_MENUKEY */
#endif	/* CONFIG_BOOTDELAY */

#if defined (CONFIG_AMIGAONEG3SE)
	{
	    extern void video_banner(void);
	    video_banner();
	}
#endif


//Need to write MIU setting if not in auto_boot flow
#if defined (CONFIG_AUTOBOOT_KEYED)
    prev = disable_ctrlc(1);    /* disable Control C checking */
#endif

    set_miu();

#if defined (CONFIG_AUTOBOOT_KEYED)
    disable_ctrlc(prev);    /* restore Control C checking */
#endif


	/*
	 * Main Loop for Monitor Command Processing
	 */
#if defined (CFG_HUSH_PARSER)
	parse_file_outer();
	/* This point is never reached */
	for (;;);
#else
	for (;;) {
#if defined (CONFIG_BOOT_RETRY_TIME)
		if (rc >= 0) {
			/* Saw enough of a valid command to
			 * restart the timeout.
			 */
			reset_cmd_timeout();
		}
#endif
		len = readline (CFG_PROMPT);

		flag = 0;	/* assume no special flags for now */
		if (len > 0)
			strcpy (lastcommand, console_buffer);
		else if (len == 0)
			flag |= CMD_FLAG_REPEAT;
#if defined (CONFIG_BOOT_RETRY_TIME)
		else if (len == -2) {
			/* -2 means timed out, retry autoboot
			 */
			puts ("\nTimed out waiting for command\n");
#ifdef (CONFIG_RESET_TO_RETRY)
			/* Reinit board to run initialization code again */
			do_reset (NULL, 0, 0, NULL);
#else
			return;		/* retry autoboot */
#endif
		}
#endif
        else
            printf("%s@%d, undefined length[%d]. \n", __func__, __LINE__, len);

		if (len == -1)
			puts ("<INTERRUPT>\n");
		else
			rc = run_command (lastcommand, flag);

		if (rc < 0 || !s_repeatable) {
			/* invalid command or not repeatable, forget it */
			lastcommand[0] = 0;
		}
	}
#endif /*CFG_HUSH_PARSER*/
#if (EANBLE_HUMMINGBIRD_CUSTOMER)

	NormalAPPEntry();
#endif

}

#if defined (CONFIG_BOOT_RETRY_TIME)
/***************************************************************************
 * initialize command line timeout
 */
void init_cmd_timeout(void)
{
	char *s = getenv ("bootretry");

	if (s != NULL)
		retry_time = (int)simple_strtol(s, NULL, 10);
	else
		retry_time =  CONFIG_BOOT_RETRY_TIME;

	if (retry_time >= 0 && retry_time < CONFIG_BOOT_RETRY_MIN)
		retry_time = CONFIG_BOOT_RETRY_MIN;
}

/***************************************************************************
 * reset command line timeout to retry_time seconds
 */
void reset_cmd_timeout(void)
{
	endtime = endtick(retry_time);
}
#endif

#if defined (CONFIG_CMDLINE_EDITING)

/*
 * cmdline-editing related codes from vivi.
 * Author: Janghoon Lyu <nandy@mizi.com>
 */

#define putnstr(str,n)	do {			\
		printf ("%.*s", n, str);	\
	} while (0)

#define CTL_CH(c)		((c) - 'a' + 1)

#define MAX_CMDBUF_SIZE		256

#define CTL_BACKSPACE		('\b')
#define DEL			((char)255)
#define DEL7			((char)127)
#define CREAD_HIST_CHAR		('!')

#define getcmd_putch(ch)	putc(ch)
#define getcmd_getch()		getc()
#define getcmd_cbeep()		getcmd_putch('\a')

#define HIST_MAX		20
#define HIST_SIZE		MAX_CMDBUF_SIZE

static int hist_max = 0;
static int hist_add_idx = 0;
static int hist_cur = -1;
unsigned hist_num = 0;

char* hist_list[HIST_MAX];
char hist_lines[HIST_MAX][HIST_SIZE];

#define add_idx_minus_one() ((hist_add_idx == 0) ? hist_max : hist_add_idx-1)

static void hist_init(void)
{
	int i;

	hist_max = 0;
	hist_add_idx = 0;
	hist_cur = -1;
	hist_num = 0;

	for (i = 0; i < HIST_MAX; i++) {
		hist_list[i] = hist_lines[i];
		hist_list[i][0] = '\0';
	}
}

static void cread_add_to_hist(char *line)
{
	strcpy(hist_list[hist_add_idx], line);

	if (++hist_add_idx >= HIST_MAX)
		hist_add_idx = 0;

	if (hist_add_idx > hist_max)
		hist_max = hist_add_idx;

	hist_num++;
}

static char* hist_prev(void)
{
	char *ret;
	int old_cur;

	if (hist_cur < 0)
		return NULL;

	old_cur = hist_cur;
	if (--hist_cur < 0)
		hist_cur = hist_max;

	if (hist_cur == hist_add_idx) {
		hist_cur = old_cur;
		ret = NULL;
	} else
		ret = hist_list[hist_cur];

	return (ret);
}

static char* hist_next(void)
{
	char *ret;

	if (hist_cur < 0)
		return NULL;

	if (hist_cur == hist_add_idx)
		return NULL;

	if (++hist_cur > hist_max)
		hist_cur = 0;

	if (hist_cur == hist_add_idx) {
		ret = "";
	} else
		ret = hist_list[hist_cur];

	return (ret);
}

#ifndef CONFIG_CMDLINE_EDITING
static void cread_print_hist_list(void)
{
	int i;
	unsigned long n;

	n = hist_num - hist_max;

	i = hist_add_idx + 1;
	while (1) {
		if (i > hist_max)
			i = 0;
		if (i == hist_add_idx)
			break;
		printf("%s\n", hist_list[i]);
		n++;
		i++;
	}
}
#endif /* CONFIG_CMDLINE_EDITING */

#define BEGINNING_OF_LINE() {			\
	while (num) {				\
		getcmd_putch(CTL_BACKSPACE);	\
		num--;				\
	}					\
}

#define ERASE_TO_EOL() {				\
	if (num < eol_num) {				\
		int tmp;				\
		for (tmp = num; tmp < eol_num; tmp++)	\
			getcmd_putch(' ');		\
		while (tmp-- > num)			\
			getcmd_putch(CTL_BACKSPACE);	\
		eol_num = num;				\
	}						\
}

#define REFRESH_TO_EOL() {			\
	if (num < eol_num) {			\
		wlen = eol_num - num;		\
		putnstr(buf + num, wlen);	\
		num = eol_num;			\
	}					\
}

static void cread_add_char(char ichar, int insert, unsigned long *num,
	       unsigned long *eol_num, char *buf, unsigned long len)
{
	unsigned long wlen;

	/* room ??? */
	if (insert || *num == *eol_num) {
		if (*eol_num > len - 1) {
			getcmd_cbeep();
			return;
		}
		(*eol_num)++;
	}

	if (insert) {
		wlen = *eol_num - *num;
		if (wlen > 1) {
			memmove(&buf[*num+1], &buf[*num], wlen-1);
		}

		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
		while (--wlen) {
			getcmd_putch(CTL_BACKSPACE);
		}
	} else {
		/* echo the character */
		wlen = 1;
		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
	}
}

static void cread_add_str(char *str, int strsize, int insert, unsigned long *num,
	      unsigned long *eol_num, char *buf, unsigned long len)
{
	while (strsize--) {
		cread_add_char(*str, insert, num, eol_num, buf, len);
		str++;
	}
}

static int cread_line(char *buf, unsigned int *len)
{
	unsigned long num = 0;
	unsigned long eol_num = 0;
	unsigned long rlen;
	unsigned long wlen;
	char ichar;
	int insert = 1;
	int esc_len = 0;
	int rc = 0;
	char esc_save[8];

	while (1) {
		rlen = 1;
		ichar = getcmd_getch();

		if ((ichar == '\n') || (ichar == '\r')) {
			putc('\n');
			break;
		}

		/*
		 * handle standard linux xterm esc sequences for arrow key, etc.
		 */
		if (esc_len != 0) {
			if (esc_len == 1) {
				if (ichar == '[') {
					esc_save[esc_len] = ichar;
					esc_len = 2;
				} else {
					cread_add_str(esc_save, esc_len, insert,
						      &num, &eol_num, buf, *len);
					esc_len = 0;
				}
				continue;
			}

			switch (ichar) {

			case 'D':	/* <- key */
				ichar = CTL_CH('b');
				esc_len = 0;
				break;
			case 'C':	/* -> key */
				ichar = CTL_CH('f');
				esc_len = 0;
				break;	/* pass off to ^F handler */
			case 'H':	/* Home key */
				ichar = CTL_CH('a');
				esc_len = 0;
				break;	/* pass off to ^A handler */
			case 'A':	/* up arrow */
				ichar = CTL_CH('p');
				esc_len = 0;
				break;	/* pass off to ^P handler */
			case 'B':	/* down arrow */
				ichar = CTL_CH('n');
				esc_len = 0;
				break;	/* pass off to ^N handler */
			default:
				esc_save[esc_len++] = ichar;
				cread_add_str(esc_save, esc_len, insert,
					      &num, &eol_num, buf, *len);
				esc_len = 0;
				continue;
			}
		}

		switch (ichar) {
		case 0x1b:
			if (esc_len == 0) {
				esc_save[esc_len] = ichar;
				esc_len = 1;
			} else {
				puts("impossible condition #876\n");
				esc_len = 0;
			}
			break;

		case CTL_CH('a'):
			BEGINNING_OF_LINE();
			break;
		case CTL_CH('c'):	/* ^C - break */
			*buf = '\0';	/* discard input */
			return (-1);
		case CTL_CH('f'):
			if (num < eol_num) {
				getcmd_putch(buf[num]);
				num++;
			}
			break;
		case CTL_CH('b'):
			if (num) {
				getcmd_putch(CTL_BACKSPACE);
				num--;
			}
			break;
		case CTL_CH('d'):
			if (num < eol_num) {
				wlen = eol_num - num - 1;
				if (wlen) {
					memmove(&buf[num], &buf[num+1], wlen);
					putnstr(buf + num, wlen);
				}

				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('k'):
			ERASE_TO_EOL();
			break;
		case CTL_CH('e'):
			REFRESH_TO_EOL();
			break;
		case CTL_CH('o'):
			insert = !insert;
			break;
		case CTL_CH('x'):
			BEGINNING_OF_LINE();
			ERASE_TO_EOL();
			break;
		case DEL:
		case DEL7:
		case 8:
			if (num) {
				wlen = eol_num - num;
				num--;
				memmove(&buf[num], &buf[num+1], wlen);
				getcmd_putch(CTL_BACKSPACE);
				putnstr(buf + num, wlen);
				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('p'):
		case CTL_CH('n'):
		{
			char * hline;

			esc_len = 0;

			if (ichar == CTL_CH('p'))
				hline = hist_prev();
			else
				hline = hist_next();

			if (!hline) {
				getcmd_cbeep();
				continue;
			}

			/* nuke the current line */
			/* first, go home */
			BEGINNING_OF_LINE();

			/* erase to end of line */
			ERASE_TO_EOL();

			/* copy new line into place and display */
			strcpy(buf, hline);
			eol_num = strlen(buf);
			REFRESH_TO_EOL();
			continue;
		}
		default:
			cread_add_char(ichar, insert, &num, &eol_num, buf, *len);
			break;
		}
	}
	*len = eol_num;
	buf[eol_num] = '\0';	/* lose the newline */

	if (buf[0] && buf[0] != CREAD_HIST_CHAR)
		cread_add_to_hist(buf);
	hist_cur = hist_add_idx;

	return (rc);
}

#endif /* CONFIG_CMDLINE_EDITING */

/****************************************************************************/

/*
 * Prompt for input and read a line.
 * If  CONFIG_BOOT_RETRY_TIME is defined and retry_time >= 0,
 * time out when time goes past endtime (timebase time in ticks).
 * Return:	number of read characters
 *		-1 if break
 *		-2 if timed out
 */
int readline (const char *const prompt)
{
#if defined (CONFIG_CMDLINE_EDITING)
	char *p = console_buffer;
	unsigned int len=MAX_CMDBUF_SIZE;
	int rc;
	static int initted = 0;

	if (!initted) {
		hist_init();
		initted = 1;
	}

	printf (prompt);

	rc = cread_line(p, &len);
	return rc < 0 ? rc : len;
#else
	char   *p = console_buffer;
	int	n = 0;				/* buffer index		*/
	int	plen = 0;			/* prompt length	*/
	int	col;				/* output column cnt	*/
	char	c;

	/* print prompt */
	if (prompt) {
		plen = strlen (prompt);
		printf (prompt);
	}
	col = plen;

	for (;;) {
#if defined (CONFIG_BOOT_RETRY_TIME)
		while (!tstc()) {	/* while no incoming data */
			if (retry_time >= 0 && get_ticks() > endtime)
				return (-2);	/* timed out */
		}
#endif
		WATCHDOG_RESET();		/* Trigger watchdog, if needed */

#if defined (CONFIG_SHOW_ACTIVITY)
		while (!tstc()) {
			extern void show_activity(int arg);
			show_activity(0);
		}
#endif
		c = getc();

		/*
		 * Special character handling
		 */
		switch (c) {
		case '\r':				/* Enter		*/
		case '\n':
			*p = '\0';
			printf ("\r\n");
			return (p - console_buffer);

		case '\0':				/* nul			*/
			continue;

		case 0x03:				/* ^C - break		*/
			console_buffer[0] = '\0';	/* discard input */
			return (-1);

		case 0x15:				/* ^U - erase line	*/
			while (col > plen) {
				printf (erase_seq);
				--col;
			}
			p = console_buffer;
			n = 0;
			continue;

		case 0x17:				/* ^W - erase word 	*/
			p=delete_char(console_buffer, p, &col, &n, plen);
			while ((n > 0) && (*p != ' ')) {
				p=delete_char(console_buffer, p, &col, &n, plen);
			}
			continue;

		case 0x08:				/* ^H  - backspace	*/
		case 0x7F:				/* DEL - backspace	*/
			p=delete_char(console_buffer, p, &col, &n, plen);
			continue;

		default:
			/*
			 * Must be a normal character then
			 */
			if (n < CFG_CBSIZE-2) {
				if (c == '\t') {	/* expand TABs		*/
#if defined (CONFIG_AUTO_COMPLETE)
					/* if auto completion triggered just continue */
					*p = '\0';
					if (cmd_auto_complete(prompt, console_buffer, &n, &col)) {
						p = console_buffer + n;	/* reset */
						continue;
					}
#endif
					printf (tab_seq+(col&07));
					col += 8 - (col&07);
				} else {
					++col;		/* echo input		*/
					putc (c);
				}
				*p++ = c;
				++n;
			} else {			/* Buffer full		*/
				putc ('\a');
			}
		}
	}
#endif /* CONFIG_CMDLINE_EDITING */
}

/****************************************************************************/

#ifndef CONFIG_CMDLINE_EDITING
static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen)
{
	char *s;

	if (*np == 0) {
		return (p);
	}

	if (*(--p) == '\t') {			/* will retype the whole line	*/
		while (*colp > plen) {
			printf (erase_seq);
			(*colp)--;
		}
		for (s=buffer; s<p; ++s) {
			if (*s == '\t') {
				printf (tab_seq+((*colp) & 07));
				*colp += 8 - ((*colp) & 07);
			} else {
				++(*colp);
				putc (*s);
			}
		}
	} else {
		printf (erase_seq);
		(*colp)--;
	}
	(*np)--;
	return (p);
}
#endif /* CONFIG_CMDLINE_EDITING */

/****************************************************************************/

int parse_line (char *line, char *argv[])
{
	int nargs = 0;

#if defined (DEBUG_PARSER)
	printf ("parse_line: \"%s\"\n", line);
#endif
	while (nargs < CFG_MAXARGS)
    {
		/* skip any white space */
		while ((*line == ' ') || (*line == '\t'))
        {
			++line;
		}

		if (*line == '\0')
        {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#if defined (DEBUG_PARSER)
		printf ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && (*line != ' ') && (*line != '\t'))
        {
			++line;
		}

		if (*line == '\0')
        {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#if defined (DEBUG_PARSER)
		printf ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	printf ("** Too many args (max. %d) **\n", CFG_MAXARGS);

#if defined (DEBUG_PARSER)
	printf ("4. parse_line: nargs=%d\n", nargs);
#endif
	return (nargs);
}

/****************************************************************************/
static void process_macros (const char *input, char *output)
{
	char c, prev;
	const char *varname_start = NULL;
	int inputcnt = strlen (input);
	int outputcnt = CFG_CBSIZE;
	int state = 0;		/* 0 = waiting for '$'  */

	/* 1 = waiting for '(' or '{' */
	/* 2 = waiting for ')' or '}' */
	/* 3 = waiting for '''  */
#if defined (DEBUG_PARSER)
	char *output_start = output;
	printf ("[PROCESS_MACROS] INPUT len %d: \"%s\"\n", strlen (input), input);
#endif

	prev = '\0';		/* previous character   */

	while (inputcnt && outputcnt)
    {
		c = *input++;
		inputcnt--;

		if (state != 3)
        {
			/* remove one level of escape characters */
			if ((c == '\\') && (prev != '\\'))
            {
				if (inputcnt-- == 0)
					break;
				prev = c;
				c = *input++;
			}
		}

		switch (state)
        {
    		case 0:	/* Waiting for (unescaped) $    */
    			if ((c == '\'') && (prev != '\\'))
                {
    				state = 3;
    				break;
    			}

    			if ((c == '$') && (prev != '\\'))
                {
    				state++;
    			}
                else
    		    {
    				*(output++) = c;
    				outputcnt--;
    			}
    			break;

    		case 1:	/* Waiting for (        */
    			if (c == '(' || c == '{')
                {
    				state++;
    				varname_start = input;
    			}
                else
                {
    				state = 0;
    				*(output++) = '$';
    				outputcnt--;

    				if (outputcnt)
                    {
    					*(output++) = c;
    					outputcnt--;
    				}
    			}
    			break;

    		case 2:	/* Waiting for )        */
    			if (c == ')' || c == '}')
                {
    				int i;
    				char envname[CFG_CBSIZE], *envval;
    				int envcnt = input - varname_start - 1;	/* Varname # of chars */

    				/* Get the varname */
    				for (i = 0; i < envcnt; i++)
                    {
    					envname[i] = varname_start[i];
    				}
    				envname[i] = 0;

    				/* Get its value */
    				envval = getenv (envname);

    				/* Copy into the line if it exists */
    				if (envval != NULL)
    				{
    					while ((*envval) && outputcnt)
                        {
    						*(output++) = *(envval++);
    						outputcnt--;
    					}
    				}
    				/* Look for another '$' */
    				state = 0;
    			}
    			break;

    		case 3:	/* Waiting for '        */
    			if ((c == '\'') && (prev != '\\'))
                {
    				state = 0;
    			}
                else
                {
    				*(output++) = c;
    				outputcnt--;
    			}
    			break;

            default:
                break;
        }
		prev = c;
	}

	if (outputcnt)
	{
		*output = 0;
	}

#if defined (DEBUG_PARSER)
	printf ("[PROCESS_MACROS] OUTPUT len %d: \"%s\"\n", strlen (output_start), output_start);
#endif
}

/****************************************************************************
 * returns:
 *	1  - command executed, repeatable
 *	0  - command executed but not repeatable, interrupted commands are
 *	     always considered not repeatable
 *	-1 - not executed (unrecognized, bootd recursion or too many args)
 *           (If cmd is NULL or "" or longer than CFG_CBSIZE-1 it is
 *           considered unrecognized)
 *
 * WARNING:
 *
 * We must create a temporary copy of the command since the command we get
 * may be the result from getenv(), which returns a pointer directly to
 * the environment data, which may change magicly when the command we run
 * creates or modifies environment variables (like "bootp" does).
 */

int run_command (const char *cmd, int flag)
{
	cmd_tbl_t *cmdtp;
	char cmdbuf[CFG_CBSIZE];	/* working copy of cmd		*/
	char *token;			/* start of token in cmdbuf	*/
	char *sep;			/* end of token (separator) in cmdbuf */
	char finaltoken[CFG_CBSIZE];
	char *str = cmdbuf;
	char *argv[CFG_MAXARGS + 1];	/* NULL terminated	*/
	int argc, inquotes;
	int rc = -1;

#if defined (DEBUG_PARSER)
	printf ("[RUN_COMMAND] cmd[%p]=\"", cmd);
	puts (cmd ? cmd : "NULL");	/* use puts - string may be loooong */
	puts ("\"\n");
#endif

	clear_ctrlc();		/* forget any previous Control C */

	if (!cmd || !*cmd)
    {
		return -1;	/* empty command */
	}

	if (strlen(cmd) >= CFG_CBSIZE)
    {
		puts ("## Command too long!\n");
		return -1;
	}

	strcpy (cmdbuf, cmd);

	s_repeatable = 1;

	/* Process separators and check for invalid
	 * repeatable commands
	 */

#if defined (DEBUG_PARSER)
	printf ("[PROCESS_SEPARATORS] %s\n", cmd);
#endif
	while (*str)
    {

		/*
		 * Find separator, or string end
		 * Allow simple escape of ';' by writing "\;"
		 */
		for (inquotes = 0, sep = str; *sep; sep++)
        {
			if ((*sep=='\'') && (*(sep-1) != '\\'))
			{
				inquotes=!inquotes;
			}

			if (!inquotes &&
			    (*sep == ';') &&	/* separator		*/
			    ( sep != str) &&	/* past string start	*/
			    (*(sep-1) != '\\'))	/* and NOT escaped	*/
				break;
		}

		/*
		 * Limit the token to data between separators
		 */
		token = str;
		if (*sep)
        {
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
		}
		else
		{
			str = sep;	/* no more commands for next pass */
		}

#if defined (DEBUG_PARSER)
		printf ("token: \"%s\"\n", token);
#endif

		/* find macros in this token and replace them */
		process_macros (token, finaltoken);

		/* Extract arguments */
		if ((argc = parse_line (finaltoken, argv)) == 0)
        {
            /* no command at all */
			continue;
		}

		/* Look up command in command table */
		if ((cmdtp = find_cmd(argv[0])) == NULL)
        {
			printf ("Unknown command '%s' - try 'help'\n", argv[0]);
			/* give up after bad command */
			continue;
		}

		/* found - check max args */
		if (argc > cmdtp->maxargs)
        {
			printf ("Usage:\n%s\n", cmdtp->usage);
			continue;
		}

#if (CONFIG_COMMANDS & CFG_CMD_BOOTD)
		/* avoid "bootd" recursion */
		if (cmdtp->cmd == do_bootd)
        {
#if defined (DEBUG_PARSER)
			printf ("[%s]\n", finaltoken);
#endif
			if (flag & CMD_FLAG_BOOTD)
            {
				puts ("'bootd' recursion detected\n");
				continue;
			}
            else
			{
				flag |= CMD_FLAG_BOOTD;
			}
		}
#endif	/* CFG_CMD_BOOTD */

		/* OK - call function to do the command */
		if (0 == (cmdtp->cmd) (cmdtp, flag, argc, argv))
        {
			rc = 0;
		}

		s_repeatable &= cmdtp->repeatable;

		/* Did the user stop this? */
		if (had_ctrlc ())
		{
			return 0;	/* if stopped then not repeatable */
		}
	}

    return rc;
}

#if (CONFIG_COMMANDS & CFG_CMD_RUN)
int do_run (cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int i;

	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	for (i=1; i<argc; ++i) {
		char *arg;

		if ((arg = getenv (argv[i])) == NULL) {
			printf ("## Error: \"%s\" not defined\n", argv[i]);
			return 1;
		}
#ifndef CFG_HUSH_PARSER
		if (run_command (arg, flag) == -1)
			return 1;
#else
		if (parse_string_outer(arg,
		    FLAG_PARSE_SEMICOLON | FLAG_EXIT_FROM_LOOP) != 0)
			return 1;
#endif
	}
	return 0;
}



// ------------------------------------------------------------------------
//
//  Add Customer Specific Command BELOW (CUSTOMIZATION)
//
// ------------------------------------------------------------------------

// Define (CUSTOMIZATION)
#define ENV_BOOTDELAY               "bootdelay"
#define ENV_IPADDR                  "ipaddr"
#define ENV_SERVERIP                "serverip"
#define ENV_PARTNO                  "partno"

#define DEFAULT_BOOTDELAY           "1"                 // boot delay
#define DEFAULT_IPADDR              "192.168.0.28"      // target (debug) board IP
#define DEFAULT_SERVERIP            "192.168.0.26"      // tftp server IP
#define DEFAULT_SCRIPT_FILE_NAME    "auto_update.txt"   // script file name

//Upgrade define
#define USB_SCRIPT_FILE_NAME        "usb_auto_update.txt"  //usb update script file name
#define OAD_ENV_FILE_NAME           "oad.txt"  //usb update script file name
#define OAD_FILE_NAME               "oad.bin"
#define ENV_OAD                     "oad_complete"
#define ENV_USB                     "usb_complete"
#define INFO_EXCHANGE_CFG           "info_exchange"
#define INFO_EXCHANGE_UBIFILE       "ubifile"
#define INFO_EXCHANGE_SPI           "spi"
#define USB_ENV_FILE_NAME           "usb.txt"
#define MAX_USB_PATH_LEN            256
#define USB_UPGRADE                 "usb_upgrade"
#define USB_UPGRADE_PORT            "usb_upgrade_port"
#define USB_UPGRADE_PATH            "usb_upgrade_path"

#define SCRIPT_FILE_COMMENT         '#'                 // symbol for comment
#define SCRIPT_FILE_END             '%'                 // symbol for file end

#define ARG_NUM_BOOTDELAY           4
#define ARG_NUM_IPADDR              2
#define ARG_NUM_SERVERIP            3
#define ARG_NUM_SCRIPT_FILE         1

// Macro
#define MAX_LINE_SIZE       CFG_CBSIZE
#define IS_COMMENT(x)       (SCRIPT_FILE_COMMENT == (x))
#define IS_FILE_END(x)      (SCRIPT_FILE_END == (x))
#define IS_LINE_END(x)      ('\r' == (x)|| '\n' == (x))

#define IS_ARG_AVAILABLE_BOOTDELAY(x)   ((x) > ARG_NUM_BOOTDELAY)
#define IS_ARG_AVAILABLE_IPADDR(x)      ((x) > ARG_NUM_IPADDR)
#define IS_ARG_AVAILABLE_SERVERIP(x)    ((x) > ARG_NUM_SERVERIP)
#define IS_ARG_AVAILABLE_SCRIPT_FILE(x) ((x) > ARG_NUM_SCRIPT_FILE)

#define ARG_BOOTDELAY(x)                (x)[ARG_NUM_BOOTDELAY]
#define ARG_IPADDR(x)                   (x)[ARG_NUM_IPADDR]
#define ARG_SERVERIP(x)                 (x)[ARG_NUM_SERVERIP]
#define ARG_SCRIPT_FILE(x)              (x)[ARG_NUM_SCRIPT_FILE]

#ifdef CFG_INFO_IS_IN_UBI_FILE
typedef struct
{
    U8 u8UsbPort;
    char cPath[MAX_USB_PATH_LEN + 1];

} USB_UpgradeInfo;
#endif

// Local Function
static char *get_script_next_line(char **line_buf_ptr)
{
    char *line_buf;
    char *next_line;
    int i;

    line_buf = (*line_buf_ptr);

    // strip '\r', '\n' and comment
    while (1)
    {
        // strip '\r' & '\n'
        if (IS_LINE_END(line_buf[0]))
        {
            line_buf++;
        }
        // strip comment
        else if (IS_COMMENT(line_buf[0]))
        {
            for (i = 0; !IS_LINE_END(line_buf[0]) && i <= MAX_LINE_SIZE; i++)
            {
                line_buf++;
            }

            if (i > MAX_LINE_SIZE)
            {
                line_buf[0] = SCRIPT_FILE_END;

                printf ("Error: the max size of one line is %d!!!\n", MAX_LINE_SIZE); // <-@@@

                break;
            }
        }
        else
        {
            break;
        }
    }

    // get next line
    if (IS_FILE_END(line_buf[0]))
    {
        next_line = NULL;
    }
    else
    {
        next_line = line_buf;

        for (i = 0; !IS_LINE_END(line_buf[0]) && i <= MAX_LINE_SIZE; i++)
        {
            line_buf++;
        }

        if (i > MAX_LINE_SIZE)
        {
            next_line = NULL;

            printf ("Error: the max size of one line is %d!!!\n", MAX_LINE_SIZE); // <-@@@
        }
        else
        {
            line_buf[0] = '\0';
            *line_buf_ptr = line_buf + 1;
        }
    }

    return next_line;
}

unsigned short ascii_to_hex(const char *buffer, int length)
{
    unsigned short u16temp;
    U8 tempbuffer[4];
    U8 i = 0;

    while(i<4)
    {
        //printf("%c ", *buffer);
        if((*buffer >= 0x30)&&(*buffer <= 0x39)) // 0 ~ 9
            tempbuffer[i] = *buffer - 0x30;
        else if((*buffer >= 0x41)&&(*buffer <= 0x46)) // A ~ E
            tempbuffer[i] = *buffer - 0x37;
        else if((*buffer >= 0x61)&&(*buffer <= 0x66)) // a ~ e
            tempbuffer[i] = *buffer - 0x57;
        else
            printf("Error> wrong character\n");

        //printf("0x%x \n", tempbuffer[i]);
        i++;
        buffer++;
    }

    u16temp = (U16)(tempbuffer[0]<<12|tempbuffer[1]<<8|tempbuffer[2]<<4|tempbuffer[3]);
    //printf("%04x\n", u16temp);

    return u16temp;
}

static void set_miu_group_priority(U8 u8MiuIdx, const char* cStr)
{
    if (cStr)
    {
        int i = 0;
        int j = 0;
        int length = 0;
        U8 au8GpPriority[4] = {0};

        for (;;)
        {
            if (cStr[i] != ':')
            {
                au8GpPriority[j++] = cStr[i] - 0x30;
            }
            else
            {
                ++length; // length is ':' number and priority number need add one more.
            }

            if ('\0' == cStr[i])
            {
                ++length;
                break;
            }

            ++i;
        }

        MDrv_MIU_SetGroupPriority_UBoot(u8MiuIdx, au8GpPriority, length);
    }
}

static void select_miu(U8 u8MiuIdx, const char* cStr)
{
    if (cStr)
    {
        int i = 0;
        int j = 0;
        int length = 0;

        for (;;)
        {
            if (':' == cStr[i])
            {
                ++length;
            }

            if ('\0' == cStr[i])
            {
                ++length; // length is ':' number and priority number need add one more.
                break;
            }

            ++i;
        }

        i = 0;
        j = 0;
        U16 au16SelMiu[4] = {0};

        for (;;)
        {
            au16SelMiu[j++] = ascii_to_hex(&cStr[i], 4);
            i += 5;

            if (length == j)
            {
                break;
            }
        }

        MDrv_MIU_SelectMIU_UBoot (u8MiuIdx, au16SelMiu, length);
    }
}

void set_miu4boot(void)
{
    select_miu(0, "0000:0000:0000");
    select_miu(1, "0000:0000:0000");
}

static void set_miu(void)
{
    // set default value
#if (CONFIG_TITANIA4)
    U8 au8GpPriorityDefault[4] = {0, 1, 2, 3};
    MDrv_MIU_SetGroupPriority_UBoot(0, au8GpPriorityDefault, 4);
    MDrv_MIU_SetGroupPriority_UBoot(1, au8GpPriorityDefault, 4);
#endif

    set_miu_group_priority(0, getenv("MIU0_GROUP_PRIORITY"));
    set_miu_group_priority(1, getenv("MIU1_GROUP_PRIORITY"));
    select_miu(0, getenv("MIU0_GROUP_SELMIU"));
    select_miu(1, getenv("MIU1_GROUP_SELMIU"));
}

#if (defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA4)|| defined(CONFIG_TITANIA13)) && (CONFIG_COMMANDS & CFG_CMD_SECURE)
int do_encrypt_Test (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    MS_U32 difftime=0;

    if(argc!=3)
    {
        Secure_AES_Performance(0x200, E_DRVAESDMA_CIPHER_ECB, TRUE);
        return TRUE;
    }

    if (strcmp(argv[1],"aesecb") == 0)
    {
        difftime=Secure_AES_Performance(simple_strtoul (argv[2], NULL, 10), E_DRVAESDMA_CIPHER_ECB, TRUE);
    }
    else if (strcmp(argv[1],"aescbc") == 0)
    {
        difftime=Secure_AES_Performance(simple_strtoul (argv[2], NULL, 10), E_DRVAESDMA_CIPHER_CBC, TRUE);
    }
    else if (strcmp(argv[1],"aesctr") == 0)
    {
        difftime=Secure_AES_Performance(simple_strtoul (argv[2], NULL, 10), E_DRVAESDMA_CIPHER_CTR, TRUE);
    }

    printf("usage time: %08d ms\n",difftime);

    return TRUE;
}

int do_decrypt_Test (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    MS_U32 difftime=0;

    if(argc!=3)
    {
        Secure_AES_Performance(0x200, E_DRVAESDMA_CIPHER_ECB, FALSE);
        return TRUE;
    }

    if (strcmp(argv[1],"aesecb") == 0)
    {
        difftime=Secure_AES_Performance(simple_strtoul (argv[2], NULL, 10), E_DRVAESDMA_CIPHER_ECB, FALSE);
    }
    else if (strcmp(argv[1],"aescbc") == 0)
    {
        difftime=Secure_AES_Performance(simple_strtoul (argv[2], NULL, 10), E_DRVAESDMA_CIPHER_CBC, FALSE);
    }
    else if (strcmp(argv[1],"aesctr") == 0)
    {
        difftime=Secure_AES_Performance(simple_strtoul (argv[2], NULL, 10), E_DRVAESDMA_CIPHER_CTR, FALSE);
    }

    printf("usage time: %08d ms\n",difftime);

    return TRUE;
}

extern DRVAESDMA_RESULT CommonSHA(MS_U32 u32InBuf, MS_U32 u32OutBuf, MS_U32 u32Len);
int do_SHA_Test (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    CommonSHA(0x80400000, 0x80400000, 0x400000);

    return TRUE;
}

#include <sys/drv/drvMBX.h>
#include <sys/drv/drvBDMA.h>
#include <sys/drv/drvPM.h>

typedef struct
{
    U16 u16ChipID;
    U8 u8ChipVersion;
    U8 u8ChipRevision;
}ChipParamStruct;

U8 PM51_status = FALSE;
extern MS_BOOL MApi_MBX_Init(void);

void PM51_Init(void)
{
    MBX_Result mbxResult;

    MApi_MBX_Init();

    MDrv_BDMA_SetSPIOffsetForMCU();

    mbxResult = MDrv_MBX_RegisterMSG(E_MBX_CLASS_PM_WAIT, 6);
    if(E_MBX_SUCCESS != mbxResult)
    {
        printf("E_MBX_CLASS_PM_WAIT - MBX register msg error\n");
    }
}

int do_getChipID_Test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    ChipParamStruct ChipParam;

    if(PM51_status == FALSE)
    {
        PM51_Init();
        PM51_status = TRUE;
    }

    MDrv_PM_GetChipID((MS_U8 *)&ChipParam);

    printf("%04X %02X %02X\n", ChipParam.u16ChipID, ChipParam.u8ChipVersion, ChipParam.u8ChipRevision);

    return TRUE;
}

int do_getDeviceID_Test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    U8 DeviceID[8];
    U8 i;

    if(PM51_status == FALSE)
    {
        PM51_Init();
        PM51_status = TRUE;
    }

    MDrv_PM_GetDeviceID(&DeviceID[0]);

    for(i=0;i<6;i++)
    {
        printf("%02X ", DeviceID[i]);
    }
    printf("\n");

    return TRUE;
}
#endif

#if defined(CONFIG_USB_STORAGE)
static void check_usb_upgrade(void)
{
    ulong port = 0;
    char* pPath = NULL;
    char buffer[100] = {0};

    //printf("%s@%d, config_USB_STORAGE. \n", __FUNCTION__, __LINE__ );

#ifdef CFG_INFO_IS_IN_UBI_FILE
    sprintf(buffer, "ubi part UBI");
    if (run_command(buffer, 0) >= 0)
    {
        sprintf(buffer, "ubifsmount RFS");
        if (run_command(buffer, 0) >= 0)
        {
            sprintf(buffer, "ubifsload %X %s", UPGRADE_BUFFER_ADDR, USB_ENV_FILE_NAME);
            if (run_command(buffer, 0) >= 0)
            {
                USB_UpgradeInfo* pInfo = (USB_UpgradeInfo*)UPGRADE_BUFFER_ADDR;
                port = pInfo->u8UsbPort;
                pPath = pInfo->cPath;
            }
        }
    }

    //printf("%s@%d, CFG_INFO_IS_IN_UBI_FILE. \n", __FUNCTION__, __LINE__ );
    //FIXME!!! Patch for ubi bug.
    run_command("ubi reinit", 0);

#elif (CFG_INFO_IS_IN_SPI_ENV || CFG_INFO_IS_IN_NAND_ENV)
    char* cUsbUpgrade = getenv(USB_UPGRADE);
    if (!cUsbUpgrade)
    {
        return;
    }

    if (simple_strtoul (cUsbUpgrade, NULL, 10) != 1)
    {
        return;
    }

    char* cUsbPort = getenv(USB_UPGRADE_PORT);
    if (cUsbPort)
    {
        port = simple_strtoul(cUsbPort, NULL, 10);
    }

    pPath = getenv(USB_UPGRADE_PATH);
    //printf("%s@%d, USB_upgrade_path[%s]. \n", __FUNCTION__, __LINE__, pPath);
#else
    //printf("%s@%d, other case. \n", __FUNCTION__, __LINE__ );
    #error
#endif

   //scan usable usb port
    for(port = 0; port < 3; port++)
    {
        if (usb_init(port) >= 0)
        {
            break;
        }
    }

    //printf("%s@%d, USB_upgrade_path[%s]. \n", __FUNCTION__, __LINE__, pPath);
    if (!pPath)
    {
        return;
    }

    if (port > 2)
    {
        return;
    }

    if (usb_stor_scan(1) != 0)
    {
        //printf("%s@%d, USB_storage_scan() found USB storage device num > 0. \n", __FUNCTION__, __LINE__);
        return;
    }

    //printf("%s@%d, prepare to run ustar, USB_UPGRADE_Path[%s]. \n", __FUNCTION__, __LINE__, pPath);
    sprintf(buffer, "ustar %s", pPath);
    s_usbUpgrade = 1;
    run_command(buffer, 0);
    s_usbUpgrade = 0;

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "osd_destroy");
    run_command(buffer, 0);
}
#endif

static void set_info_exchange_cfg(void)
{
#ifdef CFG_INFO_IS_IN_UBI_FILE
    char* pInfo = getenv(INFO_EXCHANGE_CFG);
    if (!pInfo || strcmp(pInfo, INFO_EXCHANGE_UBIFILE) != 0)
    {
        setenv(INFO_EXCHANGE_CFG, INFO_EXCHANGE_UBIFILE);
        saveenv();
    }
#elif CFG_INFO_IS_IN_SPI_ENV
    char* pInfo = getenv(INFO_EXCHANGE_CFG);
    if (!pInfo || strcmp(pInfo, INFO_EXCHANGE_SPI) != 0)
    {
        setenv(INFO_EXCHANGE_CFG, INFO_EXCHANGE_SPI);
        saveenv();
    }
#endif
}

//Set env cfg to bootargs
static void set_env_cfg_2bootargs(void)
{
    char* pArgs = getenv("bootargs");
    if (pArgs)
    {
        size_t len = strlen(pArgs);
        char* pOldArgs = (char*)malloc(len + 1);
        strcpy(pOldArgs, pArgs);

        //if ENV_CFG exist, delete it.
        char* pPreEnv = strstr(pOldArgs, ENV_CFG_PREFIX);
        if (pPreEnv)
        {
            char* pPreEnvEnd = strchr(pPreEnv, ' ');
            if (pPreEnvEnd)
            {
                int remain = len - (++pPreEnvEnd - pOldArgs);
                if (remain > 0)
                {
                    char* pRemainBuf = (char*)malloc(remain + 1);
                    strcpy(pRemainBuf, pPreEnvEnd);
                    strcpy(pPreEnv, pRemainBuf);
                    free(pRemainBuf);
                }
                else
                {
                    *pPreEnv = 0;
                }
            }
            else
            {
                *pPreEnv = 0;
            }
        }

        char* pNewArgs = (char*)malloc(len + 1 + sizeof(ENV_CFG));
        if (pOldArgs[strlen(pOldArgs) - 1] != ' ')
        {
            sprintf(pNewArgs, "%s %s ", pOldArgs, ENV_CFG);
        }
        else
        {
            sprintf(pNewArgs, "%s%s ", pOldArgs, ENV_CFG);
        }

        setenv("bootargs", pNewArgs);
        free(pNewArgs);
        free(pOldArgs);
    }
}

int do_mstar (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char* buffer=NULL;
    buffer=(char *)malloc(BUF_SIZE);
    if(buffer==NULL)
    {
        printf("no memory for command string!!\n");
        return -1;
    }

    // setenv (prelude)
    if (IS_ARG_AVAILABLE_BOOTDELAY(argc) || getenv(ENV_BOOTDELAY) == NULL)
    {
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "setenv %s %s", ENV_BOOTDELAY, IS_ARG_AVAILABLE_BOOTDELAY(argc) ? ARG_BOOTDELAY(argv) : DEFAULT_BOOTDELAY);
        run_command(buffer, 0); // run_command("setenv "ENV_BOOTDELAY" "DEFAULT_BOOTDELAY, 0);
    }
    if (IS_ARG_AVAILABLE_IPADDR(argc) || getenv(ENV_IPADDR) == NULL)
    {
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "setenv %s %s", ENV_IPADDR, IS_ARG_AVAILABLE_IPADDR(argc) ? ARG_IPADDR(argv) : DEFAULT_IPADDR);
        run_command(buffer, 0); // run_command("setenv "ENV_IPADDR" "DEFAULT_IPADDR, 0);
    }
    if (IS_ARG_AVAILABLE_SERVERIP(argc) || getenv(ENV_SERVERIP) == NULL)
    {
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "setenv %s %s", ENV_SERVERIP, IS_ARG_AVAILABLE_SERVERIP(argc) ? ARG_SERVERIP(argv) : DEFAULT_SERVERIP);
        run_command(buffer, 0); // run_command("setenv "ENV_SERVERIP" "DEFAULT_SERVERIP, 0);
    }

    // load & run script
    {
        char *script_buf;
        char *next_line;
#if defined (ENABLE_USB_LAN_MODULE)
	#if (ENABLE_MSTAR_TITANIA_BD_MST090F_C01A)		//should refine it later
        run_command("usb start 1",0);
	#else
        run_command("usb start",0);
	#endif
#else
        run_command("estart", 0);
#endif
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "tftp %X %s", (U32)buffer, IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_SCRIPT_FILE_NAME);
        run_command(buffer, 0); // run_command("tftp 80400000 "DEFAULT_SCRIPT_FILE_NAME, 0);
        script_buf = buffer;
        while ((next_line = get_script_next_line(&script_buf)) != NULL)
        {
            printf ("\n>> %s \n", next_line);
            run_command(next_line, 0);
        }
    }
    free(buffer);

    return 0;
}
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
unsigned int gCustomerOUI;
unsigned int gHWModel;
unsigned int gHWVersion;
unsigned int gAppSwModel;
unsigned int gAppSwVersion;
unsigned int gCountryType = COUNTRY_TYPE_EUROPE;
char gChipVersion[10]="";
char gBoardSel[10]="";
unsigned int gBuildCL=0;

MS_BOOL MApi_LDR_CheckValidImage(unsigned int u32CusOUI, unsigned int u32SwModel, unsigned int u32SwVer, char* cChipVersion, char* cBoardSel, unsigned int u32BuildCL)
{
#if	(ENABLE_USB_UPGRADE_CHECK_LEVEL1)
    if (u32CusOUI == gCustomerOUI && u32SwModel == gAppSwModel && u32SwVer >= gAppSwVersion && strcmp(cChipVersion,gChipVersion) == 0 && strcmp(cBoardSel,gBoardSel) == 0 )
#elif (ENABLE_USB_UPGRADE_CHECK_LEVEL2)
    if (u32CusOUI == gCustomerOUI && u32SwModel == gAppSwModel && u32SwVer >= gAppSwVersion && strcmp(cChipVersion,gChipVersion) == 0 && strcmp(cBoardSel,gBoardSel) == 0 && u32BuildCL >= gBuildCL)
#endif
	{
        return TRUE;
    }

    else
    {
        printf("MApi_LDR_CheckValidImage(%x,%x,%x,%s,%s,%d), Invalid\n", gCustomerOUI, gAppSwModel, gAppSwVersion,gChipVersion,gBoardSel,gBuildCL);
        return 0;
    }
}


static unsigned char gIsCusInfoMatched = 0;
unsigned char MApi_LDR_IsCusInfoMatched(void)
{
    return gIsCusInfoMatched;
}

int do_checkOUIValid(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    unsigned int oui;
    unsigned int swModel;
    unsigned int swVer;
    char chip_version[10];
    char board_sel[10];
    unsigned int buildCL;

    if(argc!=7)
    {
        printf("too few arguments %d to check, please check header in bin file!!\n",argc);
        return -1;
    }

    oui = simple_strtoul(argv[1], NULL, 16);
    swModel = simple_strtoul(argv[2], NULL, 16);
    swVer   = simple_strtoul(argv[3], NULL, 16);
    
    strcpy(chip_version,argv[4]);
    strcpy(board_sel,argv[5]);
    buildCL = atoi(argv[6]);

	MApi_LDR_GetOADBasicInfo();

    if (MApi_LDR_CheckValidImage(oui, swModel, swVer,chip_version,board_sel,buildCL))
    {
        gIsCusInfoMatched = 1;
        return 0;
    }
    return -1;
}
MS_BOOL MApi_LDR_GetOADBasicInfo(void)
{
    char *pTemp;

    pTemp = getenv("CUSTOMER_OUI");
    if (pTemp)
    {
        gCustomerOUI = simple_strtoul(pTemp, NULL, 16);
    }

    pTemp = getenv("AP_SW_MODEL");
    if (pTemp)
    {
        gAppSwModel = simple_strtoul(pTemp, NULL, 16);
    }

    pTemp = getenv("AP_SW_VERSION");
    if (pTemp)
    {
        gAppSwVersion = simple_strtoul(pTemp, NULL, 16);
    }

    pTemp = getenv("HW_MODEL");
    if (pTemp)
    {
        gHWModel = simple_strtoul(pTemp, NULL, 16);
    }

    pTemp = getenv("HW_VERSION");
    if (pTemp)
    {
        gHWVersion = simple_strtoul(pTemp, NULL, 16);
    }

    pTemp = getenv("COUNTRY_TYPE");
    if (pTemp)
    {
        gCountryType = simple_strtoul(pTemp, NULL, 16);
    }

    pTemp = getenv("CHIP_VERSION");
    if (pTemp)
    {
        strcpy(gChipVersion,pTemp);
    }
    
    pTemp = getenv("MS_BOARD_TYPE_SEL");
    if (pTemp)
    {
        strcpy(gBoardSel,pTemp);
    }
    
    pTemp = getenv("BUILDCL");
    if (pTemp)
    {
        gBuildCL = simple_strtoul(pTemp, NULL, 16);
    }

    return TRUE;
}

#endif //end ENABLE_HUMMINGBIRD_CUSTOMER
/*******************************************************************************************************************/
///[In]cmdtp: Monitor Command Table
///[In]flag:
///[In]argc:parm number
///[In]argv[]:input command
///This function to use usb update
/*******************************************************************************************************************/
int do_ustar(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char* buffer=NULL;
    buffer=(char *)malloc(BUF_SIZE);
    int ret=10;
    char * cUsb= getenv(ENV_USB);
    ulong  usb_complete = 0;

    if(buffer==NULL)
    {
        printf("no memory for command string!!\n");
        return -1;
    }

    //printf("%s@%d, USB envParam[%s]. \n", __FUNCTION__, __LINE__, cUsb);
    if(cUsb==NULL)
    {
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "setenv %s 0", ENV_USB);
        run_command(buffer, 0); // run_command("setenv usb_complete 0");

        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "saveenv");
        run_command(buffer, 0); // run_command("saveenv");
    }
    else
    {
        usb_complete = simple_strtoul (cUsb, NULL, 10);
        if(usb_complete==1)
        {
            memset(buffer, 0 , BUF_SIZE);
            sprintf(buffer, "setenv %s 0", ENV_USB);
            run_command(buffer, 0);  // run_command("setenv usb_complete 0");

            memset(buffer, 0 , BUF_SIZE);
            sprintf(buffer, "saveenv");
            run_command(buffer, 0); // run_command("saveenv");
            free(buffer);
            printf("last USB upgrade complete\n");
            return 0;
        }
    }

    // setenv (prelude)
    if (IS_ARG_AVAILABLE_BOOTDELAY(argc) || getenv(ENV_BOOTDELAY) == NULL)
    {
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "setenv %s %s", ENV_BOOTDELAY, IS_ARG_AVAILABLE_BOOTDELAY(argc) ? ARG_BOOTDELAY(argv) : DEFAULT_BOOTDELAY);
        run_command(buffer, 0); // run_command("setenv "ENV_BOOTDELAY" "DEFAULT_BOOTDELAY, 0);
    }

    // load & run script
    {
        char *script_buf;
        char *next_line;


        {
            // Check if file exist by load 1 byte of the file
            if (s_usbUpgrade)
            {
                sprintf(buffer, "fatload usb 0  %X %s 1", UPGRADE_BUFFER_ADDR, IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : USB_SCRIPT_FILE_NAME);
            }
            else
            {
                //For backward compatible, we must check "USB_SCRIPT_FILE_NAME" in USB disk first
                sprintf(buffer, "fatload usb 0  %X /Target/script/%s 1", (U32)buffer, IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : USB_SCRIPT_FILE_NAME);
            }

            ret = run_command(buffer, 0);
            if(ret!=0)
            {
                printf("cmd %s failed\n", buffer);
                goto exit;
            }
        }

#if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        U8 progress_cnt = 0;

        if (s_usbUpgrade)
        {
            sprintf(buffer, "osd_create %d %d", GWIN_WIDTH, GWIN_HEIGHT);
            run_command(buffer, 0);
            sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
            run_command(buffer, 0);
            sprintf(buffer, "draw_string %d %d 0x3fffffff 1 LOADING DATA...", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
            run_command(buffer, 0);
            progress_cnt = 1;
            sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
            run_command(buffer, 0);
        }
#endif

        if (s_usbUpgrade)
        {
            sprintf(buffer, "fatload usb 0  %X %s", UPGRADE_BUFFER_ADDR, IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : USB_SCRIPT_FILE_NAME);
        }
        else
        {
            //For backward compatible, we must check "USB_SCRIPT_FILE_NAME" in USB disk first
            sprintf(buffer, "fatload usb 0  %X /Target/script/%s", (U32)buffer, IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : USB_SCRIPT_FILE_NAME);
        }

        ret = run_command(buffer, 0);

        if(s_usbUpgrade)
        {
            // Check CRC
            U32 CRC_result=0;
            U32 CRC_value=0;
            U32 addr=0;
            U32 file_size=0;

            file_size = simple_strtol(getenv("filesize"), NULL, 16);
            if((file_size <= 4) || (file_size%4 != 0))
            {
                //wrong file size
                printf("wrong file size: 0x%lx\n", file_size);
                goto exit;
            }
            addr = UPGRADE_BUFFER_ADDR+(file_size-4);
            CRC_value = CRC_result+1;
            if(addr > UPGRADE_BUFFER_ADDR)
            {
                CRC_value=*((U32*)(addr));
            }
            CRC_result=crc32( 0x00, (U8*)UPGRADE_BUFFER_ADDR, file_size-4 );
            if(CRC_result!= CRC_value)
            {
                //CRC error
                printf("wrong file size: 0x%lx\n", file_size);
#if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
            {
                memset(buffer, 0 , 256);
                sprintf(buffer, "draw_rect %d %d %d %d 000000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
                ret=run_command(buffer, 0);

                memset(buffer, 0 , 256);
                sprintf(buffer, "draw_string %d %d ffffffff 1 '%s'", GRAPHIC_X, GRAPHIC_Y+200, "Software Update(USB)");
                ret=run_command(buffer, 0);

                memset(buffer, 0 , 256);
                sprintf(buffer, "draw_string %d %d ffffffff 1 '%s'", GRAPHIC_X, GRAPHIC_Y+300, "Wrong CRC");
                ret=run_command(buffer, 0);
                udelay(1000000);
            }
#endif //(CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
                goto exit;
            }
        }

#if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        if (s_usbUpgrade)
        {
            sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
            run_command(buffer, 0);
            sprintf(buffer, "draw_string %d %d 0x3fffffff 1 UPGRADING SOFTWARE", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
            run_command(buffer, 0);
            sprintf(buffer, "draw_string %d %d 0x3fffffff 1 PLEASE DO NOT TURN OFF", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 3);
            run_command(buffer, 0);
            progress_cnt = 40;
            sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
            run_command(buffer, 0);
        }
#endif

        //if "usb_auto_update.txt" does not exist, check "USB_UPGRADE_BIN" in RFS
        if((ret!=0)&&(IS_ARG_AVAILABLE_SCRIPT_FILE(argc)==0))  //check "USB_UPGRADE_BIN"
        {
#if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
            memset(buffer, 0 , BUF_SIZE);
            sprintf(buffer, "ubi part UBI");
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
                printf("cmd %s failed\n", buffer);
                goto exit;
            }

            memset(buffer, 0 , BUF_SIZE);
            sprintf(buffer, "ubifsmount RFS");
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
                printf("cmd %s failed\n", buffer);
                goto exit;
            }

            memset(buffer, 0 , BUF_SIZE);
            sprintf(buffer, "ubifsload %X %s", (U32)buffer, IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : USB_SCRIPT_FILE_NAME);
            ret = run_command(buffer, 0);
            if(ret==-1)
            {
                printf("cmd %s failed\n", buffer);
                goto exit;
            }
#else
            sprintf(buffer, "chpart RFS");
            ret = run_command(buffer, 0); // run_command("setenv "ENV_BOOTDELAY" "DEFAULT_BOOTDELAY, 0);
            if (ret < 0)
            {
                printf("cmd %s failed\n", buffer);
                goto exit;
            }

            sprintf(buffer, "fsload %X %s", (U32)buffer, IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : USB_SCRIPT_FILE_NAME);
            ret = run_command(buffer, 0); // run_command("fsload "USB_UPGRADE_BIN");
            if (ret < 0)
            {
                printf("cmd %s failed\n", buffer);
                goto exit;
            }
#endif
        }

        //ret = run_command(buffer, 0); // run_command("fatload usb 0 80400000 "USB_SCRIPT_FILE_NAME", 0);
        script_buf = s_usbUpgrade ? (char *)UPGRADE_BUFFER_ADDR : buffer;

        while ((next_line = get_script_next_line(&script_buf)) != NULL)
        {
            printf ("\n>> %s \n", next_line);
#if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
            if (s_usbUpgrade)
            {
                if (strncmp("reset", next_line, 5) != 0)
                {
                    if (progress_cnt < 100)
                    {
                        progress_cnt += 4;
                        sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 5, GRAPHIC_WIDTH, LINE_HEIGHT);
                        run_command(buffer, 0);
                        sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
                        run_command(buffer, 0);
                    }
                }
                else
                {
                    progress_cnt = 100;
                    sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
                    run_command(buffer, 0);
                    sprintf(buffer, "draw_string %d %d 0x3fffffff 1 SOFTWARE UPGRADE COMPLETE", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
                    run_command(buffer, 0);
                    sprintf(buffer, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
                    run_command(buffer, 0);
                    udelay(1000000);
                    sprintf(buffer, "osd_destroy"); //clear screen
                    run_command(buffer, 0);
                }
            }
#endif
#ifdef CFG_INFO_IS_IN_UBI_FILE
            //patch for USB upgrade needs twice init
            if (strncmp("reset", next_line, 5) == 0)
            {
                memset(buffer, 0 , BUF_SIZE);
                sprintf(buffer, "setenv %s 0", ENV_USB);
                run_command(buffer, 0); // run_command("setenv usb_complete 0");

                memset(buffer, 0 , BUF_SIZE);
                sprintf(buffer, "saveenv");
                run_command(buffer, 0); // run_command("saveenv");
            }
#endif //CFG_INFO_IS_IN_UBI_FILE
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
//added for twice init
if (strncmp("reset", next_line, 5) == 0)
{
	memset(buffer, 0 , BUF_SIZE);
	sprintf(buffer, "setenv %s 0", ENV_USB);
	run_command(buffer, 0); // run_command("setenv usb_complete 0");

	memset(buffer, 0 , BUF_SIZE);
	sprintf(buffer, "saveenv");
	run_command(buffer, 0); // run_command("saveenv");
}

//added end
#endif //end ENABLE_HUMMINGBIRD_CUSTOMER

#if WDT_ENABLE_HB
    MDrv_WDT_Stop(E_WDT_DBGLV_INFO);
#endif

            ret = run_command(next_line, 0);
            if (ret < 0)
            {
                printf("Error> cmd: %s FAIL !!\n", next_line);
#if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
                if (s_usbUpgrade)
                {
                    sprintf(buffer, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
                    run_command(buffer, 0);
                    sprintf(buffer, "draw_string %d %d 0x3fffffff 1 SOFTWARE UPGRADE ERROR", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
                    run_command(buffer, 0);
                    //udelay(5000000);
                    //sprintf(buffer, "reset");
                    //run_command(buffer, 0);
                }
#endif
                break;
            }
        }
    }

exit:
    free(buffer);
    return ret;
}

/*******************************************************************************************************************/
///[In]cmdtp: Monitor Command Table
///[In]flag:
///[In]argc:parm number
///[In]argv[]:input command
///This function to use usb update
/*******************************************************************************************************************/

#if defined (CONFIG_OAD)
#if ((CONFIG_COMMANDS & CFG_CMD_UBI) && (CONFIG_COMMANDS & CFG_CMD_UBIFS))
#if defined (CONFIG_OAD_IN_MBOOT)
static MS_BOOL check_oad_upgrade(U32 *freq, U16 *pid, U8 *band)
#else
static MS_BOOL check_oad_upgrade(void)
#endif
{
#define BUFFER_SIZE 100
    char buffer[BUFFER_SIZE] = {0};
    char *cOad= NULL;
    ulong  oad_complete=0;

    //check if ENV_OAD == 1
    cOad = getenv(ENV_OAD);
    if(cOad==NULL)
    {
        memset(buffer, 0 , BUFFER_SIZE);
        sprintf(buffer, "setenv %s 0", ENV_OAD);
        run_command(buffer, 0);  // run_command("setenv oad_complete 0");

        memset(buffer, 0 , BUFFER_SIZE);
        sprintf(buffer, "saveenv");
        run_command(buffer, 0); // run_command("saveenv");

        //Get environment variable "ENV_OAD" again if it was NULL originally.
        cOad = getenv(ENV_OAD);
        if(cOad == NULL)
        {
            printf("Reset environment '%s' failed\n", ENV_OAD);
            return FALSE;
        }
    }
    oad_complete = simple_strtoul (cOad, NULL, 10);
    if(oad_complete==1)
    {
        memset(buffer, 0 , BUFFER_SIZE);
        sprintf(buffer, "setenv %s 0", ENV_OAD);
        run_command(buffer, 0);  // run_command("setenv oad_complete 0");
        memset(buffer, 0 , BUFFER_SIZE);
        sprintf(buffer, "saveenv");
        run_command(buffer, 0); // run_command("saveenv");
#if (!CONFIG_MSTAR_SILENT)
        printf("last OAD upgrade complete\n");
#endif
        return FALSE;
    }

#ifdef CFG_INFO_IS_IN_UBI_FILE
#undef RETURN_ERR
#define RETURN_ERR(a)  {run_command("ubi reinit", 0);return a;}
#undef RETURN_OK
#define RETURN_OK(a)  {run_command("ubi reinit", 0);return a;}

    sprintf(buffer, "ubi part UBI");
    if (run_command(buffer, 0) >= 0)
    {
        sprintf(buffer, "ubifsmount oad");
        if (run_command(buffer, 0) >= 0)
        {
            sprintf(buffer, "ubifsload %X %s", UPGRADE_BUFFER_ADDR, OAD_ENV_FILE_NAME);
            if (run_command(buffer, 0) >= 0)
            {
#if defined (CONFIG_OAD_IN_MBOOT)
                //check if needed to do MBoot OAD
                unsigned char *pData = (unsigned char*)UPGRADE_BUFFER_ADDR;
                if((pData[0]=='O') && (pData[1]=='A') && (pData[2]=='D'))
                {
                    RETURN_ERR(FALSE);
                }
                else
                {
                    *freq=pData[0]|(pData[1]<<8)|(pData[2]<<16)|(pData[3]<<24);
                    *pid=pData[4]|(pData[5]<<8);
                    *band=pData[6];
                    if((*band<1) || (*band>3))
                    {
                        printf("%s: wrong band: %u, at %d\n", __func__, *band, __LINE__);
                        RETURN_ERR(FALSE);
                    }
                    RETURN_OK(TRUE);
                }
#endif //defined (CONFIG_OAD_IN_MBOOT)
                RETURN_OK(TRUE);
            }
        }
    }

    //FIXME!!! Patch for ubi bug.
    RETURN_ERR(FALSE);

#elif CFG_INFO_IS_IN_SPI_ENV //CFG_INFO_IS_IN_UBI_FILE
    char* p_str = getenv("OAD_NEED_UPGRADE");
    if (!p_str)
    {
        return FALSE;
    }
    if (simple_strtoul (p_str, NULL, 10) != 1)
    {
        return FALSE;
    }
#if defined (CONFIG_OAD_IN_MBOOT)
    p_str = getenv("OAD_FREQ");
    if(p_str == NULL)
        return FALSE;
    *freq = simple_strtoul (p_str, NULL, 10);
    p_str = getenv("OAD_PID");
    if(p_str == NULL)
        return FALSE;
    *pid = simple_strtoul (p_str, NULL, 16);
    p_str = getenv("OAD_BAND");
    if(p_str == NULL)
        return FALSE;
    *band = simple_strtoul (p_str, NULL, 10);
    if((*band<1) || (*band>3))
    {
        printf("%s: wrong band: %u, at %d\n", __func__, *band, __LINE__);
        return FALSE;
    }
#endif //defined (CONFIG_OAD_IN_MBOOT)
    return TRUE;
#else //CFG_INFO_IS_IN_UBI_FILE
    #error "Wrong CFG_INFO!"
#endif //CFG_INFO_IS_IN_UBI_FILE
    return FALSE;
}
#endif //((CONFIG_COMMANDS & CFG_CMD_UBI) && (CONFIG_COMMANDS & CFG_CMD_UBIFS))
#endif //CONFIG_OAD

int do_ostar(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if defined (CONFIG_OAD)
#if ((CONFIG_COMMANDS & CFG_CMD_UBI) && (CONFIG_COMMANDS & CFG_CMD_UBIFS))
    char* buffer=NULL;
    int ret=10;
    U32 file_size=0;
    U32 CRC_result=0;
    U32 CRC_value=0;
#if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
#if defined (CONFIG_OAD_IN_MBOOT)
    U8 cmd_loop_cnt=0;
#endif //defined (CONFIG_OAD_IN_MBOOT)
#endif
#undef RETURN_ERR
#define RETURN_ERR(a)  {run_command("ubi reinit", 0);free(buffer);return a;}
#undef RETURN_OK
#define RETURN_OK(a)  {run_command("ubi reinit", 0);free(buffer);return a;}

#if (!CONFIG_MSTAR_SILENT)
    printf("do_ostar\n");
#endif

    buffer=(char *)malloc(BUF_SIZE);
    if(buffer==NULL)
    {
        printf("no memory for command string!!\n");
        return -1;
    }

    // setenv (prelude)
    if (IS_ARG_AVAILABLE_BOOTDELAY(argc) || getenv(ENV_BOOTDELAY) == NULL)
    {
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "setenv %s %s", ENV_BOOTDELAY, IS_ARG_AVAILABLE_BOOTDELAY(argc) ? ARG_BOOTDELAY(argv) : DEFAULT_BOOTDELAY);
        run_command(buffer, 0); // run_command("setenv "ENV_BOOTDELAY" "DEFAULT_BOOTDELAY, 0);
    }

    // load & run script
    {
#if defined (CONFIG_SUPERNOVA_OAD)
        char *script_buf;
        char *next_line;
#if defined (CONFIG_OAD_IN_MBOOT)
        U32 u32Freq=0;
        U16 u16Pid=0;
        U8 u8Band=0;
#endif //(CONFIG_OAD_IN_MBOOT)

#if defined (CONFIG_OAD_IN_MBOOT)
        {
            printf("%s,%d: do_ostar(), CONFIG_OAD_IN_MBOOT[%d].\n", __func__, __LINE__, CONFIG_OAD_IN_MBOOT);
            //check if needed to do MBoot OAD
            if(check_oad_upgrade(&u32Freq, &u16Pid, &u8Band) == FALSE)
            {
                RETURN_ERR(-1);
            }
        }
        if(abortboot(1))
            RETURN_ERR(-1);

        //set ENV_OAD 1. TO reboot w/o OAD next time (if download fail, reboot and enter ap without another try)
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "setenv %s 1", ENV_OAD);
        run_command(buffer, 0);  // run_command("setenv oad_complete 0");
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "saveenv");
        run_command(buffer, 0); // run_command("saveenv");

        memset(buffer, 0 , 256);
        sprintf(buffer, "oad %lu %x 0x%x", u32Freq, u8Band, u16Pid); //do OAD: download from ts to memory
        ret=run_command(buffer, 0);
        if(ret==-1)
        {
            run_command("reset", 0); //drivers have been initialized, need reset
            RETURN_ERR(-1);
        }

        memset(buffer, 0 , 256);
        sprintf(buffer, "oad_get_size 0x%08x", &file_size);
        ret=run_command(buffer, 0);
        if(ret==-1)
            RETURN_ERR(-1);

        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "ubi part UBI");
        ret = run_command(buffer, 0);
        if(ret==-1)
            RETURN_ERR(-1);

        //set ENV_OAD 0. TO reboot with OAD next time (flash erase/write may fail)
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "setenv %s 0", ENV_OAD);
        run_command(buffer, 0);  // run_command("setenv oad_complete 0");
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "saveenv");
        run_command(buffer, 0); // run_command("saveenv");

#else //defined (CONFIG_OAD_IN_MBOOT)
        printf("%s,%d: do_ostar(), 0 != CONFIG_OAD_IN_MBOOT[%d].\n", __func__, __LINE__, CONFIG_OAD_IN_MBOOT);
        //check if needed to do OAD
        if(check_oad_upgrade() == FALSE)
        {
            RETURN_ERR(-1);
        }
        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "ubi part UBI");
        ret = run_command(buffer, 0);
        if(ret==-1)
            RETURN_ERR(-1);

        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "ubifsmount oad");
        ret = run_command(buffer, 0);
        if(ret==-1)
            RETURN_ERR(-1);

        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "ubifsload %X %s", UPGRADE_BUFFER_ADDR,  OAD_FILE_NAME);
        ret = run_command(buffer, 0); // run_command("ubifsload 80400000 "OAD_FILE_NAME");
        if(ret==-1)
            RETURN_ERR(-1);

        memset(buffer, 0 , 256);
        sprintf(buffer, "ubifsfilesize %s 0x%08x", OAD_FILE_NAME, &file_size);
        ret=run_command(buffer, 0);
        if(ret==-1)
            RETURN_ERR(-1);
#if (!CONFIG_MSTAR_SILENT)
        printf("%s: file_size: 0x%lX, at %d\n", __func__, file_size, __LINE__);
#endif
#endif //defined (CONFIG_OAD_IN_MBOOT)

        if((file_size <= 4) || (file_size%4 != 0))
        {
#if (!CONFIG_MSTAR_SILENT)
            if(file_size <= 4)
                printf("file size incorrect(%lu)\n", file_size);
            else
                printf("%s: file size alignment is wrong: 0x%lX, at %d\n", __func__, file_size%4, __LINE__);
#endif
            RETURN_ERR(-1);
        }

        //CRC
        CRC_result=crc32( 0x00, (U8*)UPGRADE_BUFFER_ADDR, file_size-4 );
#elif defined (CONFIG_OBAMA_OAD) //defined (CONFIG_SUPERNOVA_OAD)
        char *script_buf;
        char *next_line;

        memset(buffer, 0 , 256);
        sprintf(buffer, "ubi part UBI");
        ret=run_command(buffer, 0);
        if(ret==-1)
            RETURN_ERR(-1);

        memset(buffer, 0 , 256);
        sprintf(buffer, "ubifsmount RFS");
        ret=run_command(buffer, 0);
        if(ret==-1)
            RETURN_ERR(-1);

        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "ubifsload %X %s", (U32)buffer,  OAD_ENV_FILE_NAME);
        ret = run_command(buffer, 0);
        if(ret==-1)
            RETURN_ERR(-1);

        script_buf = (char*)buffer;
        while ((next_line = get_script_next_line(&script_buf)) != NULL)
        {
            printf ("\n>> %s \n", next_line);
            ret = run_command(next_line, 0);
            if(ret < 0)
            {
                printf("Error> cmd: %s FAIL !!\n", next_line);
                break;
            }
        }

        memset(buffer, 0 , BUF_SIZE);
        sprintf(buffer, "ubifsload %X %s", UPGRADE_BUFFER_ADDR,  OAD_FILE_NAME);
        ret = run_command(buffer, 0); // run_command("ubifsload 80400000 "OAD_FILE_NAME");
        if(ret==-1)
            RETURN_ERR(-1);

        memset(buffer, 0 , 256);
        sprintf(buffer, "ubifsfilesize %s 0x%08x", OAD_FILE_NAME, &file_size);
        ret=run_command(buffer, 0);
        if(ret==-1)
            RETURN_ERR(-1);


        //CRC
#if 1
        CRC_result=crc32( 0x00, (U8*)UPGRADE_BUFFER_ADDR, file_size-4 );
       // printf("CRC1 0x%08x \n", CRC_result);

#else

    {
#define RIU_MAP2  0xBF200000
#define RIU      ((unsigned short volatile *) RIU_MAP2)
#define RIU8    ((unsigned char  volatile *) RIU_MAP2)
#define BDMA_REG_BASE 0x900
#define DST 0x83000000

    U32 i;

    for(i=0;i<9;i++)
    {
        *((U8*)(0x80400000+i))=(i+0x31);
    }

    //RIU8[(0x1006F1 )*2] = 0x40;

    RIU8[(BDMA_REG_BASE + 0x02 )*2] = 0x00;
    //while (RIU8[(BDMA_REG_BASE + 0x02)*2] & 0x01);

    //src
    i=((MS_U32)(((MS_U32)UPGRADE_BUFFER_ADDR) & 0xfffffff));
    RIU[(BDMA_REG_BASE + 0x08 )] =(U16)((U32)(0x400000));
    RIU[(BDMA_REG_BASE + 0x0A )] =(U16)(((U32)(0x400000)) >> 16);

    //dst
    RIU[(BDMA_REG_BASE + 0x0C )] = 0x00;
    RIU[(BDMA_REG_BASE + 0x0E )] = 0x00;

    //len
    RIU[(BDMA_REG_BASE + 0x10 )] = (U16)((U32)(0x9));
    RIU[(BDMA_REG_BASE + 0x12 )] = (U16)((U32)((0x9) >> 16));

    //CRC poly
    RIU[(BDMA_REG_BASE + 0x14 )] = (U16)0x1DB7;
    RIU[(BDMA_REG_BASE + 0x16 )] = (U16)0x04C1;

    //CRC seed
    RIU[(BDMA_REG_BASE + 0x18 )] = 0x0;
    RIU[(BDMA_REG_BASE + 0x1a )] = 0x0;

     //RIU[(BDMA_REG_BASE + 0x04 )] = 0x0340;
    RIU8[(BDMA_REG_BASE + 0x06 )*2] = 0x0;
    RIU[(BDMA_REG_BASE + 0x04 )] = 0x0330;
    //RIU[(BDMA_REG_BASE + 0x06 )] = 0x0400;

    RIU8[(BDMA_REG_BASE )*2] = 0x01;

    while((RIU8[(BDMA_REG_BASE + 0x02 )*2]&0x08)!=0x08)
        printf("waiting\n");

    printf("CRC  done \n");
    printf("CRC  result 0x%04x  0x%04x \n", RIU[(BDMA_REG_BASE + 0x18 )],RIU[(BDMA_REG_BASE + 0x1a )]);
     }
#endif
#else //defined (CONFIG_SUPERNOVA_OAD)
#error "Wrong OAD selection!!"
#endif //CONFIG_SUPERNOVA_OAD

        {
            U32 addr = UPGRADE_BUFFER_ADDR+(file_size-4);
            CRC_value = CRC_result+1;
            if(addr > UPGRADE_BUFFER_ADDR)
            {
                CRC_value=*((U32*)(addr));
            }
            else
            {
#if (!CONFIG_MSTAR_SILENT)
                printf("Wrong file size: 0x%08x\n", file_size);
#endif
            }
        }

        if(CRC_result!= CRC_value)
        {
#if (!CONFIG_MSTAR_SILENT)
            printf("CRC  FAILED, CRC_value: 0x%08x,  CRC_result: 0x%08x\n", CRC_value, CRC_result);
#endif
#if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
            {
                memset(buffer, 0 , 256);
                sprintf(buffer, "draw_rect %d %d %d %d 000000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
                ret=run_command(buffer, 0);

                memset(buffer, 0 , 256);
                sprintf(buffer, "draw_string %d %d ffffffff 1 '%s'", GRAPHIC_X, GRAPHIC_Y+200, "Software Update(OAD)");
                ret=run_command(buffer, 0);

                memset(buffer, 0 , 256);
                sprintf(buffer, "draw_string %d %d ffffffff 1 '%s'", GRAPHIC_X, GRAPHIC_Y+300, "Wrong CRC");
                ret=run_command(buffer, 0);
                if(abortboot(1))
                    RETURN_ERR(-1);
#if defined (CONFIG_SUPERNOVA_OAD)
#if defined (CONFIG_OAD_IN_MBOOT)
                //set ENV_OAD 1. TO reboot w/o OAD next time (if download fail, reboot and enter ap without another try)
                memset(buffer, 0 , BUF_SIZE);
                sprintf(buffer, "setenv %s 1", ENV_OAD);
                run_command(buffer, 0);  // run_command("setenv oad_complete 0");
                memset(buffer, 0 , BUF_SIZE);
                sprintf(buffer, "saveenv");
                run_command(buffer, 0); // run_command("saveenv");

                run_command("reset", 0); //drivers have been initialized, need reset
                RETURN_ERR(-1);
#endif //CONFIG_OAD_IN_MBOOT
#endif //CONFIG_SUPERNOVA_OAD
            }
#endif //(CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
            RETURN_ERR(-1);
        }
        script_buf = (char*)UPGRADE_BUFFER_ADDR;

        while ((next_line = get_script_next_line(&script_buf)) != NULL)
        {
            printf ("\n>> %s \n", next_line);
            ret = run_command(next_line, 0);
            if(ret < 0)
            {
                printf("Error> cmd: %s FAIL !!\n", next_line);
                break;
            }
#if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
#if defined (CONFIG_OAD_IN_MBOOT)
            {
                memset(buffer, 0 , 256);
                sprintf(buffer, "draw_string %d %d ffffffff 1 '%s'", GRAPHIC_X, GRAPHIC_Y+200, "Software Update(OAD)");
                ret=run_command(buffer, 0);

                memset(buffer, 0 , 256);
                sprintf(buffer, "draw_rect %d %d %d %d 000000ff", GRAPHIC_X, GRAPHIC_Y+300, GRAPHIC_WIDTH, GRAPHIC_HEIGHT-300);
                ret=run_command(buffer, 0);

                memset(buffer, 0 , 256);
                if(cmd_loop_cnt<100)
                {
                    cmd_loop_cnt += 5;
                }
                if(strstr(next_line, "reset")==NULL)
                {
                    sprintf(buffer, "draw_progress %d %d ffffffff %u", GRAPHIC_X+60, GRAPHIC_Y+300, cmd_loop_cnt);
                }
                else
                {
                    sprintf(buffer, "draw_progress %d %d ffffffff %u", GRAPHIC_X+60, GRAPHIC_Y+300, 100);
                }
                ret=run_command(buffer, 0);
            }
#endif //defined (CONFIG_OAD_IN_MBOOT)
#endif //(CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
        }
  }

    RETURN_OK(0);
#else //((CONFIG_COMMANDS & CFG_CMD_UBI) && (CONFIG_COMMANDS & CFG_CMD_UBIFS))
    return -1;
#endif //((CONFIG_COMMANDS & CFG_CMD_UBI) && (CONFIG_COMMANDS & CFG_CMD_UBIFS))
#else //defined (CONFIG_OAD)
    return 0;
#endif //defined (CONFIG_OAD)
}

int do_usb_miu(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char Cmd_buffer[256];
    char* buffer=NULL;
    char *script_buf;
    char *next_line;

    buffer=(char *)malloc(BUF_SIZE);
    if(buffer==NULL)
    {
        printf("no memory for miu setting command string!!\n");
        return -1;
    }

    // Read file
    memset(Cmd_buffer, 0 , 256);
    sprintf(Cmd_buffer, "fatload usb 0  0x%x /Target/script/%s", (U32)buffer, IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : USB_SCRIPT_FILE_NAME);
    printf("cmd: %s\n", Cmd_buffer);
    if(run_command(Cmd_buffer, 0) == -1)
    {
        printf("Error> read miu script file failed !!\n");
        return -1;
    }

    // Get MIU setting
    script_buf = buffer;
    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        //printf ("\n>> %s \n", next_line);
        memset(Cmd_buffer, 0 , 256);
        sprintf(Cmd_buffer, "set %s", next_line);
        printf("cmd: %s\n", Cmd_buffer);
        run_command(Cmd_buffer, 0);
    }

    // save environment
    run_command("saveenv", 0);

    free(buffer);

    return 0;
}

int do_net_miu(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char Cmd_buffer[256];
    char* buffer=NULL;
    char *script_buf;
    char *next_line;

    buffer=(char *)malloc(BUF_SIZE);
    if(buffer==NULL)
    {
        printf("no memory for miu setting command string!!\n");
        return -1;
    }

    // Read file
    memset(Cmd_buffer, 0 , 256);
    if (argc > 1)
    {
        sprintf(Cmd_buffer, "tftp 0x%x %s", (U32)buffer, argv[1]);
    }
    else
    {
        printf("USAGE : net_miu filename \nSample : net_miu mscript/t3/miu_setting.txt !!\n");
        return -1;
    }

    printf("cmd: %s\n", Cmd_buffer);
    if(run_command(Cmd_buffer, 0) == -1)
    {
        printf("Error> read miu script file failed !!\n");
        return -1;
    }

    // Get MIU setting
    script_buf = buffer;
    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        //printf ("\n>> %s \n", next_line);
        memset(Cmd_buffer, 0 , 256);
        sprintf(Cmd_buffer, "set %s", next_line);
        printf("cmd: %s\n", Cmd_buffer);
        run_command(Cmd_buffer, 0);
    }

    // save environment
    run_command("saveenv", 0);

    free(buffer);

    return 0;
}

int strtrim(char* strSrc, char* strTrimChar)
{
    U8 u8Len = 0;
    U8 u8SrcStrLen, u8TrimStrLen, i, j;

    u8SrcStrLen = strlen(strSrc);
    u8TrimStrLen = strlen(strTrimChar);

    for(i = 0; i < u8SrcStrLen; ++i)
    {
        for (j = 0; j < u8TrimStrLen; j++)
        {
            if (strSrc[i] == strTrimChar[j])
            {
                break;
            }
            else if(j == u8TrimStrLen-1)
            {
                strSrc[u8Len++] = strSrc[i];
            }
        }
    }

    strSrc[u8Len] = '\0';

    return u8Len;
}

int do_setpanelinfo (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]) // <-@@@
{


    PANEL_INFO stPanelInfo;
    static char buffer[4*1024]; // the max size of script should be less than 4K (use static variable instead of local variable (stack size issue))

    if(sizeof(PANEL_INFO)>=PANEL_INFO_NAND_LEN)
    {
        printf("error!! sizeof(PANEL_INFO) to big!!\n");
        return -1;
    }

    // load & run script
    {
        char *script_buf;
        char *next_line;
        char *token1, *token2;
        // start for ethernet
        run_command("estart", 0);

        // get the script file from tftp server
        sprintf(buffer, "tftp %X %s", &(buffer[0]), argv[1]);
        run_command(buffer, 0); // run_command("tftp 80400000 "DEFAULT_SCRIPT_FILE_NAME, 0);


        // ------------------------------------------------------------------------------------------------- //
        //  "buffer" would be used for script file buffer now and can't be used for sprintf temp buffer !!!  //
        // ------------------------------------------------------------------------------------------------- //

        // run script file
        script_buf = &(buffer[0]);
        while ((next_line = get_script_next_line(&script_buf)) != NULL)
        {

            //printf ("\n\n>> %s <<\n", next_line);

            token1 = strsep(&next_line, "=");
            if (token1 != NULL)
            {
                token2 = next_line;

                strtrim(token1, " ");
                strtrim(token2, " ");

                printf("token1: \"%s\"\n",token1);
                printf("token2 : \"%s\"\n",token2 );

                if (strcmp(token1, "u16HStart") == 0 )
                {
                    stPanelInfo.u16HStart = simple_strtol(token2, NULL, 10);
                    printf("u16HStart: %d\n", stPanelInfo.u16HStart);
                }
                else if (strcmp(token1, "u16VStart") == 0 )
                {
                    stPanelInfo.u16VStart = simple_strtol(token2, NULL, 10);
                    printf("u16VStart: %d\n", stPanelInfo.u16VStart);
                }
                else if (strcmp(token1, "u16Width") == 0 )
                {
                    stPanelInfo.u16Width = simple_strtol(token2, NULL, 10);
                    printf("u16Width: %d\n", stPanelInfo.u16Width);
                }
                else if (strcmp(token1, "u16Height") == 0 )
                {
                    stPanelInfo.u16Height = simple_strtol(token2, NULL, 10);
                    printf("u16Height: %d\n", stPanelInfo.u16Height);
                }
                else if (strcmp(token1, "u16HTotal") == 0 )
                {
                    stPanelInfo.u16HTotal = simple_strtol(token2, NULL, 10);
                    printf("u16HTotal: %d\n", stPanelInfo.u16HTotal);
                }
                else if (strcmp(token1, "u16VTotal") == 0 )
                {
                    stPanelInfo.u16VTotal = simple_strtol(token2, NULL, 10);
                    printf("u16VTotal: %d\n", stPanelInfo.u16VTotal);
                }
                else if (strcmp(token1, "u16DE_VStart") == 0 )
                {
                    stPanelInfo.u16DE_VStart = simple_strtol(token2, NULL, 10);
                    printf("u16DE_VStart: %d\n", stPanelInfo.u16DE_VStart);
                }
                else if (strcmp(token1, "u32DClkFactor") == 0 )
                {
                    stPanelInfo.u32DClkFactor = simple_strtol(token2, NULL, 10);
                    printf("u32DClkFactor: %d\n", stPanelInfo.u32DClkFactor);
                }
                else if (strcmp(token1, "u16DefaultVFreq") == 0 )
                {
                    stPanelInfo.u16DefaultVFreq = simple_strtol(token2, NULL, 10);
                    printf("u16DefaultVFreq: %d\n", stPanelInfo.u16DefaultVFreq);
                }
                else if (strcmp(token1, "u16LPLL_InputDiv") == 0 )
                {
                    stPanelInfo.u16LPLL_InputDiv = simple_strtol(token2, NULL, 10);
                    printf("u16LPLL_InputDiv: %d\n", stPanelInfo.u16LPLL_InputDiv);
                }
                else if (strcmp(token1, "u16LPLL_LoopDiv") == 0 )
                {
                    stPanelInfo.u16LPLL_LoopDiv = simple_strtol(token2, NULL, 10);
                    printf("u16LPLL_LoopDiv: %d\n", stPanelInfo.u16LPLL_LoopDiv);
                }
                else if (strcmp(token1, "u16LPLL_OutputDiv") == 0 )
                {
                    stPanelInfo.u16LPLL_OutputDiv = simple_strtol(token2, NULL, 10);
                    printf("u16LPLL_OutputDiv: %d\n", stPanelInfo.u16LPLL_OutputDiv);
                }
                else if (strcmp(token1, "u8LPLL_Type") == 0 )
                {
                    stPanelInfo.u8LPLL_Type = simple_strtol(token2, NULL, 10);
                    printf("u8LPLL_Type: %d\n", stPanelInfo.u8LPLL_Type);
                }
                else if (strcmp(token1, "u8LPLL_Mode") == 0 )
                {
                    stPanelInfo.u8LPLL_Mode = simple_strtol(token2, NULL, 10);
                    printf("u8LPLL_Mode: %d\n", stPanelInfo.u8LPLL_Mode);
                }
                else if (strcmp(token1, "u8HSyncWidth") == 0 )
                {
                    stPanelInfo.u8HSyncWidth = simple_strtol(token2, NULL, 10);
                    printf("u8HSyncWidth: %d\n", stPanelInfo.u8HSyncWidth);
                }
                else if (strcmp(token1, "bPnlDblVSync") == 0 )
                {
                    if (strcmp(token2, "T") == 0)
                        stPanelInfo.bPnlDblVSync = 1;
                    else
                        stPanelInfo.bPnlDblVSync = 0;
                    printf("bPnlDblVSync: %d\n", stPanelInfo.bPnlDblVSync);
                }
                else if (strcmp(token1, "u16OCTRL") == 0 )
                {
                    stPanelInfo.u16OCTRL = simple_strtol(token2, NULL, 10);
                    printf("u16OCTRL: %d\n", stPanelInfo.u16OCTRL);
                }
                else if (strcmp(token1, "u16OSTRL") == 0 )
                {
                    stPanelInfo.u16OSTRL = simple_strtol(token2, NULL, 10);
                    printf("u16OSTRL: %d\n", stPanelInfo.u16OSTRL);
                }
                else if (strcmp(token1, "u16ODRV") == 0 )
                {
                    stPanelInfo.u16ODRV = simple_strtol(token2, NULL, 10);
                    printf("u16ODRV: %d\n", stPanelInfo.u16ODRV);
                }
                else if (strcmp(token1, "u16DITHCTRL") == 0 )
                {
                    stPanelInfo.u16DITHCTRL = simple_strtol(token2, NULL, 10);
                    printf("u16DITHCTRL: %d\n", stPanelInfo.u16DITHCTRL);
                }
                else if (strcmp(token1, "u16MOD_CTRL0") == 0 )
                {
                    stPanelInfo.u16MOD_CTRL0 = simple_strtol(token2, NULL, 10);
                    printf("u16MOD_CTRL0: %d\n", stPanelInfo.u16MOD_CTRL0);
                }
                else if (strcmp(token1, "u16MOD_CTRL9") == 0 )
                {
                    stPanelInfo.u16MOD_CTRL9 = simple_strtol(token2, NULL, 10);
                    printf("u16MOD_CTRL9: %d\n", stPanelInfo.u16MOD_CTRL9);
                }
                else if (strcmp(token1, "u16MOD_CTRLA") == 0 )
                {
                    stPanelInfo.u16MOD_CTRLA = simple_strtol(token2, NULL, 10);
                    printf("u16MOD_CTRLA: %d\n", stPanelInfo.u16MOD_CTRLA);
                }
                else if (strcmp(token1, "u8MOD_CTRLB") == 0 )
                {
                    stPanelInfo.u8MOD_CTRLB = simple_strtol(token2, NULL, 10);
                    printf("u8MOD_CTRLB: %d\n", stPanelInfo.u8MOD_CTRLB);
                }
                else if (strcmp(token1, "u8LVDSTxChannel") == 0 )
                {
                    stPanelInfo.u8LVDSTxChannel = simple_strtol(token2, NULL, 10);
                    printf("u8LVDSTxChannel: %d\n", stPanelInfo.u8LVDSTxChannel);
                }
                else if (strcmp(token1, "u8LVDSTxBitNum") == 0 )
                {
                    stPanelInfo.u8LVDSTxBitNum = simple_strtol(token2, NULL, 10);
                    printf("u8LVDSTxBitNum: %d\n", stPanelInfo.u8LVDSTxBitNum);
                }
                else if (strcmp(token1, "u8LVDSTxTiMode") == 0 )
                {
                    stPanelInfo.u8LVDSTxTiMode = simple_strtol(token2, NULL, 10);
                    printf("u8LVDSTxTiMode: %d\n", stPanelInfo.u8LVDSTxTiMode);
                }
                else if (strcmp(token1, "u8LVDSTxSwapMsbLsb") == 0 )
                {
                    stPanelInfo.u8LVDSTxSwapMsbLsb = simple_strtol(token2, NULL, 10);
                    printf("u8LVDSTxSwapMsbLsb: %d\n", stPanelInfo.u8LVDSTxSwapMsbLsb);
                }
                else if (strcmp(token1, "u8LVDSTxSwap_P_N") == 0 )
                {
                    stPanelInfo.u8LVDSTxSwap_P_N = simple_strtol(token2, NULL, 10);
                    printf("u8LVDSTxSwap_P_N: %d\n", stPanelInfo.u8LVDSTxSwap_P_N);
                }
                else if (strcmp(token1, "u8LVDSTxSwapOddEven") == 0 )
                {
                    stPanelInfo.u8LVDSTxSwapOddEven = simple_strtol(token2, NULL, 10);
                    printf("u8LVDSTxSwapOddEven: %d\n", stPanelInfo.u8LVDSTxSwapOddEven);
                }
                else if (strcmp(token1, "u8PanelVfreq") == 0 )
                {
                    stPanelInfo.u8PanelVfreq = simple_strtol(token2, NULL, 10);
                    printf("u8PanelVfreq: %d\n", stPanelInfo.u8PanelVfreq);
                }
                else if (strcmp(token1, "u8PanelChannel") == 0 )
                {
                    stPanelInfo.u8PanelChannel = simple_strtol(token2, NULL, 10);
                    printf("u8PanelChannel: %d\n", stPanelInfo.u8PanelChannel);
                }
                else if (strcmp(token1, "u8PanelLVDSSwapCH") == 0 )
                {
                    stPanelInfo.u8PanelLVDSSwapCH = simple_strtol(token2, NULL, 10);
                    printf("u8PanelLVDSSwapCH: %d\n", stPanelInfo.u8PanelLVDSSwapCH);
                }
                else if (strcmp(token1, "u8PanelBitNum") == 0 )
                {
                    stPanelInfo.u8PanelBitNum = simple_strtol(token2, NULL, 10);
                    printf("u8PanelBitNum: %d\n", stPanelInfo.u8PanelBitNum);
                }
                else if (strcmp(token1, "u8PanelLVDSShiftPair") == 0 )
                {
                    stPanelInfo.u8PanelLVDSShiftPair = simple_strtol(token2, NULL, 10);
                    printf("u8PanelLVDSShiftPair: %d\n", stPanelInfo.u8PanelLVDSShiftPair);
                }
                else if (strcmp(token1, "u8PanelLVDSTiMode") == 0 )
                {
                    stPanelInfo.u8PanelLVDSTiMode = simple_strtol(token2, NULL, 10);
                    printf("u8PanelLVDSTiMode: %d\n", stPanelInfo.u8PanelLVDSTiMode);
                }
                else if (strcmp(token1, "u8PanelLVDSSwapPol") == 0 )
                {
                    stPanelInfo.u8PanelLVDSSwapPol = simple_strtol(token2, NULL, 10);
                    printf("u8PanelLVDSSwapPol: %d\n", stPanelInfo.u8PanelLVDSSwapPol);
                }
                else if (strcmp(token1, "u8PanelLVDSSwapPair") == 0 )
                {
                    stPanelInfo.u8PanelLVDSSwapPair = simple_strtol(token2, NULL, 10);
                    printf("u8PanelLVDSSwapPair: %d\n", stPanelInfo.u8PanelLVDSSwapPair);
                }
                else if (strcmp(token1, "u8VSyncWidth") == 0 )
				{
					stPanelInfo.u8VSyncWidth = simple_strtol(token2, NULL, 10);
					printf("u8VSyncWidth: %d\n", stPanelInfo.u8VSyncWidth);
				}
                else if (strcmp(token1, "u16LVDSTxSwapValue") == 0 )
				{
					stPanelInfo.u16LVDSTxSwapValue = simple_strtol(token2, NULL, 10);
					printf("u16LVDSTxSwapValue: %d\n", stPanelInfo.u16LVDSTxSwapValue);
				}
                else if (strcmp(token1, "dclk_mode") == 0 )
                {
                    stPanelInfo.dclk_mode = simple_strtol(token2, NULL, 10);
                    printf("dclk_mode: %d\n", stPanelInfo.dclk_mode);
                }
                else if (strcmp(token1, "u16MinHTotal") == 0 )
                {
                    stPanelInfo.u16MinHTotal = simple_strtol(token2, NULL, 10);
                    printf("u16MinHTotal: %d\n", stPanelInfo.u16MinHTotal);
                }
                else if (strcmp(token1, "u16DefaultHTotal") == 0 )
                {
                    stPanelInfo.u16DefaultHTotal = simple_strtol(token2, NULL, 10);
                    printf("u16DefaultHTotal: %d\n", stPanelInfo.u16DefaultHTotal);
                }
                else if (strcmp(token1, "u16MaxHTotal") == 0 )
                {
                    stPanelInfo.u16MaxHTotal = simple_strtol(token2, NULL, 10);
                    printf("u16MaxHTotal: %d\n", stPanelInfo.u16MaxHTotal);
                }
                else if (strcmp(token1, "u16MinVTotal") == 0 )
                {
                    stPanelInfo.u16MinVTotal = simple_strtol(token2, NULL, 10);
                    printf("u16MinVTotal: %d\n", stPanelInfo.u16MinVTotal);
                }
                else if (strcmp(token1, "u16DefaultVTotal") == 0 )
                {
                    stPanelInfo.u16DefaultVTotal = simple_strtol(token2, NULL, 10);
                    printf("u16DefaultVTotal: %d\n", stPanelInfo.u16DefaultVTotal);
                }
                else if (strcmp(token1, "u16MaxVTotal") == 0 )
                {
                    stPanelInfo.u16MaxVTotal = simple_strtol(token2, NULL, 10);
                    printf("u16MaxVTotal: %d\n", stPanelInfo.u16MaxVTotal);
                }
                else if (strcmp(token1, "u32MinDCLK") == 0 )
                {
                    stPanelInfo.u32MinDCLK = simple_strtol(token2, NULL, 10);
                    printf("u32MinDCLK: %d\n", stPanelInfo.u32MinDCLK);
                }
                else if (strcmp(token1, "u32MaxDCLK") == 0 )
                {
                    stPanelInfo.u32MaxDCLK = simple_strtol(token2, NULL, 10);
                    printf("u32MaxDCLK: %d\n", stPanelInfo.u32MaxDCLK);
                }


            }
        }

        sprintf(buffer, "nand read.e %X %s %X", TEMP_BUFFER_ADDR,LINUX_KERNEL_BOOT_PARAMETER_PARTITION_NAME ,LINUX_KERNEL_BOOT_PARAMETER_PARTITION_SIZE);
        run_command(buffer, 0);

        memcpy((void*)(TEMP_BUFFER_ADDR+SYS_INFO_NAND_LEN),&stPanelInfo,sizeof(PANEL_INFO));

        sprintf(buffer, "nand erase %s %X", LINUX_KERNEL_BOOT_PARAMETER_PARTITION_NAME ,LINUX_KERNEL_BOOT_PARAMETER_PARTITION_SIZE);
        run_command(buffer, 0);

        sprintf(buffer, "nand write.e %X %s %X", TEMP_BUFFER_ADDR,LINUX_KERNEL_BOOT_PARAMETER_PARTITION_NAME ,LINUX_KERNEL_BOOT_PARAMETER_PARTITION_SIZE);
        run_command(buffer, 0);

    }

    return 0;
}

int do_setsysinfo (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]) // <-@@@
{
    SYS_INFO stSysInfo;
    static char buffer[4*1024]; // the max size of script should be less than 4K (use static variable instead of local variable (stack size issue))

    if(sizeof(SYS_INFO)>=SYS_INFO_NAND_LEN)
    {
        printf("error!! sizeof(PANEL_INFO) to big!!\n");
        return -1;
    }


    // load & run script
    {
        char *script_buf;
        char *next_line;
        char *token1, *token2;
        // start for ethernet
        run_command("estart", 0);

        // get the script file from tftp server
        sprintf(buffer, "tftp %X %s", &(buffer[0]), argv[1]);
        run_command(buffer, 0); // run_command("tftp 80400000 "DEFAULT_SCRIPT_FILE_NAME, 0);


        // ------------------------------------------------------------------------------------------------- //
        //  "buffer" would be used for script file buffer now and can't be used for sprintf temp buffer !!!  //
        // ------------------------------------------------------------------------------------------------- //

        // run script file
        script_buf = &(buffer[0]);
        while ((next_line = get_script_next_line(&script_buf)) != NULL)
        {

            //printf ("\n\n>> %s <<\n", next_line);

            token1 = strsep(&next_line, "=");
            if (token1 != NULL)
            {
                token2 = next_line;

                strtrim(token1, " ");
                strtrim(token2, " ");

                printf("token1: \"%s\"\n",token1);
                printf("token2 : \"%s\"\n",token2 );

                if (strcmp(token1, "display_controller_seperated") == 0 )
                {
                    if (strcmp(token2, "T") == 0)
                        stSysInfo.displayControllerSeperated = 1;
                    else
                        stSysInfo.displayControllerSeperated = 0;
                    printf("stSysInfo.displayControllerSeperated: %d\n", stSysInfo.displayControllerSeperated);
                }
                else if (strcmp(token1, "local_ir") == 0 )
                {
                    if (strcmp(token2, "T") == 0)
                        stSysInfo.local_ir = 1;
                    else
                        stSysInfo.local_ir = 0;
                    printf("local_ir: %d\n", stSysInfo.local_ir);
                }
                else if (strcmp(token1, "gop_index") == 0 )
                {
                    stSysInfo.gop_index = simple_strtol(token2, NULL, 10);
                    printf("gop_index: %d\n", stSysInfo.gop_index);
                }
            }
        }

        // save to nand

	sprintf(buffer, "nand read.e %X %s %X", TEMP_BUFFER_ADDR,LINUX_KERNEL_BOOT_PARAMETER_PARTITION_NAME ,LINUX_KERNEL_BOOT_PARAMETER_PARTITION_SIZE);
	run_command(buffer, 0);

	memcpy((void*)TEMP_BUFFER_ADDR,&stSysInfo,sizeof(SYS_INFO));

	sprintf(buffer, "nand erase %s %X", LINUX_KERNEL_BOOT_PARAMETER_PARTITION_NAME ,LINUX_KERNEL_BOOT_PARAMETER_PARTITION_SIZE);
	run_command(buffer, 0);

	sprintf(buffer, "nand write.e %X %s %X", TEMP_BUFFER_ADDR,LINUX_KERNEL_BOOT_PARAMETER_PARTITION_NAME ,LINUX_KERNEL_BOOT_PARAMETER_PARTITION_SIZE);
	run_command(buffer, 0);


    }
    return 0;
}

int do_cpmsbin (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    U32 u32BinAddr, u32BinSize;
    char *buffer=NULL;


    u32BinAddr = simple_strtol(getenv("MS_BIN_START"), NULL, 10);
    u32BinSize = simple_strtol(getenv("MS_BIN_SIZE"), NULL, 10);

    if(u32BinAddr==0||u32BinSize==0)
    {
        return -1;
    }

    buffer=malloc(255);
    //printf("u32BinAddr: 0x%X, u32BinSize: 0x%X\n", u32BinAddr, u32BinSize);
    if(buffer==NULL)
    {
        printf("[%s:%d]no memory!!\n",__FILE__,__LINE__);
        return -2;
    }
    memset(buffer,0,255);

    // read the file form Nand flash to DRAM
    sprintf(buffer, "nand read.e %X %s %X", (u32BinAddr|0xA0000000), CHAKRA_RES_PARTITION_NAME, u32BinSize);
    run_command(buffer, 0);
    free(buffer);

    return 0;
}
#if defined (LED_FLICK_WHEN_SW_UPDATE)
#include <sys/common/MsTypes.h>

#define RIU_MAP  0xBF000000
#define RIU8    ((unsigned char  volatile *) RIU_MAP)
#define MDrv_WriteByte(u32Reg, u8Value)     RIU8[(u32Reg << 1) - (u32Reg & 1)] = u8Value
#define MDrv_ReadByte(u32Reg)                   RIU8[(u32Reg << 1) - (u32Reg & 1)]

void do_Led_Flicker_Control(void)
{
	static U8 Ledstatus=0;
	static U32 Flicker_Control=0;

	if((get_timer(0)-Flicker_Control)>(CFG_HZ/2))
	{
		Flicker_Control=get_timer(0);
		Ledstatus = ~Ledstatus;
		if(Ledstatus)
		{
			#if((MS_BOARD_TYPE_SEL == BD_MST087F_D01A_HX)\
            ||(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_BABAOBOX)\
            ||(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_TCL))//BALL_W9
            {
			MDrv_WriteByte(0x0E20,MDrv_ReadByte(0x0E20)|BIT1);
            }
			#elif(MS_BOARD_TYPE_SEL ==BD_MST072D_SKYWORTH)//BALL_D7
			MDrv_WriteByte(0x0E20,MDrv_ReadByte(0x0E20)|BIT7);
			#endif
		}
		else
		{
			#if((MS_BOARD_TYPE_SEL == BD_MST087F_D01A_HX)\
            ||(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_BABAOBOX)\
            ||(MS_BOARD_TYPE_SEL == BD_MST087F_D01A_TCL))//BALL_W9
            {
			 MDrv_WriteByte(0x0E20,MDrv_ReadByte(0x0E20)&(~BIT1));
            }
			#elif(MS_BOARD_TYPE_SEL ==BD_MST072D_SKYWORTH)//BALL_D7
			MDrv_WriteByte(0x0E20,MDrv_ReadByte(0x0E20)&(~BIT7));
			#endif
		}
	}
}
#endif
#if defined(CONFIG_DISPLAY_LOGO)
static void update_bootlogo_bootarg()
{
    char* buffer=NULL;
    const char *BOOTLOGO_IN_MBOOT_BOOT_PARAM = "BOOTLOGO_IN_MBOOT";
    char *p_envstr=NULL;
    extern int snprintf(char *str, size_t size, const char *fmt, ...);

    buffer=(char *)malloc(BUF_SIZE);
    if(buffer==NULL)
    {
        printf("%s: no memory for command string!!, at %d\n", __func__, __LINE__);
        return ;
    }
    p_envstr = getenv("bootargs");

#if (CONFIG_COMMANDS & CFG_CMD_BOOTLOGO)
    //append UBoot BOOTLOGO param to bootargs
    if((p_envstr != NULL) && (strstr(p_envstr, BOOTLOGO_IN_MBOOT_BOOT_PARAM) == NULL))
    {
        memset(buffer, 0, BUF_SIZE);
        snprintf(buffer, BUF_SIZE, "%s %s", p_envstr, BOOTLOGO_IN_MBOOT_BOOT_PARAM);
        setenv("bootargs", buffer);
//		saveenv();
    }
#else
	if((p_envstr != NULL) && (strstr(p_envstr, BOOTLOGO_IN_MBOOT_BOOT_PARAM) != NULL))
	{
		//Remove UBoot BOOTLOGO param to bootargs
		U8 len = strstr(p_envstr, BOOTLOGO_IN_MBOOT_BOOT_PARAM)-p_envstr;
		memset(buffer, 0, BUF_SIZE);
		snprintf(buffer, len, "%s", p_envstr); //copy str in front of BOOTLOGO_IN_MBOOT_BOOT_PARAM
		snprintf(buffer+len, (BUF_SIZE-len), "%s", strstr(p_envstr, BOOTLOGO_IN_MBOOT_BOOT_PARAM)+strlen(BOOTLOGO_IN_MBOOT_BOOT_PARAM)); ////append str in back of BOOTLOGO_IN_MBOOT_BOOT_PARAM
		setenv("bootargs", buffer);
//		saveenv();
	}
#endif
    free(buffer);
}
#endif
#if defined (CONFIG_OAD)
static void update_bootarg()
{
#ifdef CFG_INFO_IS_IN_UBI_FILE
#if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
    char* buffer=NULL;
    const char *OAD_IN_MBOOT_BOOT_PARAM = "OAD_IN_MBOOT";
    char *p_envstr=NULL;
    buffer=(char *)malloc(BUF_SIZE);
    if(buffer==NULL)
    {
        printf("%s: no memory for command string!!, at %d\n", __func__, __LINE__);
        return ;
    }
    p_envstr = getenv("bootargs");
#if defined (CONFIG_OAD_IN_MBOOT)
    //append UBoot OAD param to bootargs
    if((p_envstr != NULL) && (strstr(p_envstr, OAD_IN_MBOOT_BOOT_PARAM) == NULL))
    {
        memset(buffer, 0, BUF_SIZE);
        sprintf(buffer, "%s %s", p_envstr, OAD_IN_MBOOT_BOOT_PARAM);
        setenv("bootargs", buffer);
    }
    free(buffer);
    return;
#endif //(CONFIG_OAD_IN_MBOOT)
    if((p_envstr != NULL) && (strstr(p_envstr, OAD_IN_MBOOT_BOOT_PARAM) != NULL))
    {
        //Remove UBoot OAD param to bootargs
        extern int snprintf(char *str, size_t size, const char *fmt, ...);
        U8 len = strstr(p_envstr, OAD_IN_MBOOT_BOOT_PARAM)-p_envstr;
        memset(buffer, 0, BUF_SIZE);
        snprintf(buffer, len, "%s", p_envstr); //copy str in front of OAD_IN_MBOOT_BOOT_PARAM
        sprintf(buffer+len, "%s", strstr(p_envstr, OAD_IN_MBOOT_BOOT_PARAM)+strlen(OAD_IN_MBOOT_BOOT_PARAM)); ////append str in back of OAD_IN_MBOOT_BOOT_PARAM
        setenv("bootargs", buffer);
    }
    free(buffer);
#endif //defined(CONFIG_URANUS4)
#elif CFG_INFO_IS_IN_SPI_ENV //CFG_INFO_IS_IN_UBI_FILE
    {
        char strValue[8], *p_str=NULL;
        memset(strValue, 0, sizeof(strValue));
        p_str = getenv(CFG_OAD_IN_MBOOT);
#if defined (CONFIG_OAD_IN_MBOOT)
        strValue[0] = '1';
#else
        strValue[0] = '0';
#endif
        if((p_str != NULL) && (strValue[0] == p_str[0]))
        {
            return;
        }
        setenv(CFG_OAD_IN_MBOOT, strValue);
        saveenv();
    }
#else //CFG_INFO_IS_IN_UBI_FILE
#error "Wrong CFG_INFO!"
#endif //CFG_INFO_IS_IN_UBI_FILE
    return;
}
#endif //defined (CONFIG_OAD)
/*****************************************************************************************************************/




/***********************************************************************************************************************/
#endif	/* CFG_CMD_RUN */

