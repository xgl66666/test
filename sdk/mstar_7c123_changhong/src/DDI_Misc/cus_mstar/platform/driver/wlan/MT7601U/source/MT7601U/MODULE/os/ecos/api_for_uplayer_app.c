
//#include "os/rt_ecos.h"
#include <sys/types.h>
#include <cyg/kernel/kapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <pkgconf/system.h>

#include <cyg/io/flash.h>
#include <cyg/infra/diag.h>
//#include <network.h>
//#include <net/if_var.h>
//#include <ifaddrs.h>

//#include <netinet/if_ether.h>
//#include <net/if_arp.h>

#include <cyg/io/eth/netdev.h>
#include <cyg/io/eth/eth_drv.h>
#include <string.h>

// Put platform dependent declaration here
// For example, linux type definition
typedef unsigned char			UINT8;
typedef unsigned short			UINT16;
typedef unsigned int			UINT32;
typedef unsigned long long		UINT64;
typedef int				INT32;
typedef long long 			INT64;

typedef unsigned char			UCHAR;
typedef unsigned short			USHORT;
typedef unsigned int			UINT;
typedef unsigned long			ULONG;

typedef unsigned char * 		PUINT8;
typedef unsigned short *		PUINT16;
typedef unsigned int *			PUINT32;
typedef unsigned long long *	        PUINT64;
typedef int *				PINT32;
typedef long long * 			PINT64;

// modified for fixing compile warning on Sigma 8634 platform
typedef char 				STRING;
typedef signed char			CHAR;

typedef signed short			SHORT;
typedef signed int			INT;
typedef signed long			LONG;
typedef signed long long		LONGLONG;

typedef unsigned long long		ULONGLONG;

typedef unsigned char			BOOLEAN;
typedef void				VOID;

typedef char *				PSTRING;
typedef VOID *				PVOID;
typedef CHAR *				PCHAR;
typedef UCHAR * 			PUCHAR;
typedef USHORT *			PUSHORT;
typedef LONG *				PLONG;
typedef ULONG *				PULONG;
typedef UINT *				PUINT;
typedef struct eth_drv_sc	*PNET_DEV;

#define SIOCIWFIRSTPRIV 0x8BE0
#define RTPRIV_IOCTL_GSITESURVEY (SIOCIWFIRSTPRIV + 0x0D)
#define RTPRIV_IOCTL_FLAG_NODUMPMSG 0x0002	/* Notify driver cannot dump msg to stdio/stdout when run this private ioctl cmd */
#define RTPRIV_IOCTL_FLAG_NOSPACE 0x0004	/* Notify driver didn't need copy msg to caller due to the caller didn't reserve space for this cmd */


extern INT rt28xx_ap_ioctl(
	PNET_DEV	pEndDev,
	INT			cmd,
	caddr_t		pData);



typedef struct
{
	const char*	cmd;					// Input command string
	int	(*func)(int argc, char* argv[]);
	const char*	msg;					// Help message
} COMMAND_TABLE;

// Define Linux ioctl relative structure, keep only necessary things
struct iw_point
{
	PVOID		pointer;
	USHORT		length;
	USHORT		flags;
};

union iwreq_data
{
	struct iw_point data;
};

struct iwreq {
	union
	{
		char    ifrn_name[IFNAMSIZ];    /* if name, e.g. "eth0" */
	} ifr_ifrn;

	union   iwreq_data      u;
};


/*
* For up layer app get wifi connect status


int mt7601_connect_status
-1 :init fail
0 : init value
1 : Authancation on going
12:Authancation fail
13:Authancation success
2 : Assocation on going
22:Assocation fail
23:Assocation success
3 : 4-way handle shark on going (this state if for wpapsk&wpa2psk only)
32:4-way handle shark fail,key fail
33:4-way handle shark success
4 : connected

*/
extern int mt7601_connect_status;
int get_wifi_connect_status(void)
{
	return mt7601_connect_status;
}

//static char TempString[8192];
int get_scan_list(char *ifname,char * buf,int len)
{
	int cmd;
	cyg_netdevtab_entry_t *t;
	struct eth_drv_sc *sc =NULL;
	struct iwreq param;
	int found = 0;


    if ((strcmp(ifname, "ra0") != 0 )
          /*  && (strcmp(ifname, "ra1") != 0 )
            && (strcmp(ifname, "ra2") != 0 )
            && (strcmp(ifname, "ra3") != 0 )
            && (strcmp(ifname, "ra4") != 0 )
            && (strcmp(ifname, "ra5") != 0 )
            && (strcmp(ifname, "ra6") != 0 )
            && (strcmp(ifname, "ra7") != 0 )
            && (strcmp(ifname, "wds0") != 0 )
            && (strcmp(ifname, "apcli0") != 0 )
            && (strcmp(ifname, "mesh0") != 0 )*/
    )
            return  0;

    memset(&param, 0, sizeof(struct iwreq));


	cmd = RTPRIV_IOCTL_GSITESURVEY;
            param.u.data.flags |= (RTPRIV_IOCTL_FLAG_NOSPACE | RTPRIV_IOCTL_FLAG_NODUMPMSG);

	for (t = &__NETDEVTAB__[0]; t != &__NETDEVTAB_END__; t++)
	{
		sc = (struct eth_drv_sc *)t->device_instance;
		if (strcmp(sc->dev_name, ifname) == 0) {
			found =1;
			break;
	        }
	}

	if ( found == 1 )
	{
		char *TempString=NULL;
		int original_length = 0;
		int i =0;

		TempString =malloc(7168);//7*1024
		if(TempString==NULL)
			{
			diag_printf("Not enough memory for CmdIwpriv!\n");
			return 0;
			}
		 TempString[0] = '\0';



		param.u.data.pointer = &TempString[0];
		param.u.data.length = strlen(TempString);
		rt28xx_sta_ioctl(sc, (struct ifreq *)&param, cmd);

                if(param.u.data.length==0)
                {
                    free(TempString);
                    return 0;
                }


		if (param.u.data.length != original_length)
		{
			TempString[param.u.data.length] = '\0';
			for (i =0; i < param.u.data.length;i ++)
				diag_printf("%c",TempString[i]);
			strncpy(buf,TempString,min(len,param.u.data.length));
		}

		free(TempString);
        return 1;
	}

	return 0;
}

//ret 1 for RadioOn
//ret 0 for RadioOff
int get_wifi_radio_status(void)
{
    char* ifname="ra0";
    cyg_netdevtab_entry_t *t;
    struct eth_drv_sc *sc =NULL;
    int found = 0;

    for (t = &__NETDEVTAB__[0]; t != &__NETDEVTAB_END__; t++)
    {
        sc = (struct eth_drv_sc *)t->device_instance;
	    if (strcmp(sc->dev_name, ifname) == 0) {
            found =1;
            break;
        }
    }

    if(found==0)
        return 0;

    return  rt28xx_check_radio_on_off(sc);
}

int get_connected_ap_bssid(char * buf,int len)
{
    char *ifname="ra0";

	cyg_netdevtab_entry_t *t;
	struct eth_drv_sc *sc =NULL;
	struct iwreq param;
	int found = 0;

	if(len<18)
	{
        diag_printf("buf size not enough for MAC address!\n");
	    return 0;
	}

    memset(&param, 0, sizeof(struct iwreq));

	for (t = &__NETDEVTAB__[0]; t != &__NETDEVTAB_END__; t++)
	{
		sc = (struct eth_drv_sc *)t->device_instance;
		if (strcmp(sc->dev_name, ifname) == 0) {
			found =1;
			break;
	        }
	}

	if ( found == 1 )
	{
		char *TempString=NULL;

		TempString =(char *)malloc(7168);//7*1024
		if(TempString==NULL)
		{
			diag_printf("Not enough memory for CmdIwpriv!\n");
			return 0;
		}
			
	    TempString[0] = '\0';

		param.u.data.pointer = &TempString[0];
		param.u.data.length = strlen(TempString);
		rt28xx_get_connected_ap_bssid(sc, &param);

		strcpy(buf,TempString);

		free(TempString);
		
        return 1;
	}

	return 0;
}


