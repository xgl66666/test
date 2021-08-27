/*
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

/*
 * Boot support
 */
#include <common.h>
#include <command.h>
#include <net.h>

#if (CONFIG_COMMANDS & CFG_CMD_LWIP)

int do_lwip_init(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    return lwip_sys_init();
}

U_BOOT_CMD(
	lwip_init,	1,	1,	do_lwip_init,
	"lwip_init\t- do lwip init\n",
	"do lwip init"
);

int do_lwip_ping(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    return ping("192.168.0.101", 2, 64);
}

U_BOOT_CMD(
	lwip_ping,	1,	1,	do_lwip_ping,
	"lwip_init\t- do lwip init\n",
	"do lwip init"
);

extern int lwip_dns_query(char * const hostname);

const char u8Hostname[] = "www.hinet.net";

int do_lwip_dns_query(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    printf("%04d %s %s %d\n", __LINE__, __FUNCTION__, u8Hostname, argc);
    switch(argc)
    {
        case 2:
            return lwip_dns_query(argv[1]);
        break;
    }


}

U_BOOT_CMD(
	lwip_dns_query,	2,	1,	do_lwip_dns_query,
	"lwip_init\t- do lwip init\n",
	"do lwip init"
);

extern void lwip_tcp_client_init(void);

int do_lwip_tcp_client_init(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    lwip_tcp_client_init();
    return 1;
}

U_BOOT_CMD(
	lwip_tcp_test,	2,	1, do_lwip_tcp_client_init,
	"lwip_init\t- do lwip init\n",
	"do lwip init"
);



#endif	/* CFG_CMD_NET */
