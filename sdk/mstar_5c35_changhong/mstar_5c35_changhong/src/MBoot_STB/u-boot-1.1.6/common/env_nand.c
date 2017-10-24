/*
 * (C) Copyright 2004
 * Jian Zhang, Texas Instruments, jzhang@ti.com.

 * (C) Copyright 2000-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2001 Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Andreas Heppel <aheppel@sysgo.de>

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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* #define DEBUG */

#include <common.h>
#include <linux/stddef.h>
#if defined(CFG_ENV_IS_IN_NAND) /* Environment is in Nand Flash */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <nand.h>
#include <asm/errno.h>

extern unsigned int cfg_env_offset;

#if ((CONFIG_COMMANDS&(CFG_CMD_ENV|CFG_CMD_NAND)) == (CFG_CMD_ENV|CFG_CMD_NAND))
#define CMD_SAVEENV
#elif defined(CFG_ENV_OFFSET_REDUND)
#error Cannot use CFG_ENV_OFFSET_REDUND without CFG_CMD_ENV & CFG_CMD_NAND
#endif

#if defined(CFG_ENV_SIZE_REDUND) && (CFG_ENV_SIZE_REDUND != CFG_ENV_SIZE)
#error CFG_ENV_SIZE_REDUND should be the same as CFG_ENV_SIZE
#endif

#ifdef CONFIG_INFERNO
#error CONFIG_INFERNO not supported yet
#endif

#define BUF_SIZE 40

int nand_legacy_rw (struct nand_chip* nand, int cmd,
	    size_t start, size_t len,
	    size_t * retlen, u_char * buf);

/* info for NAND chips, defined in drivers/nand/nand.c */
extern nand_info_t nand_info[];

/* references to names in env_common.c */
extern uchar default_environment[];
extern int default_environment_size;

extern int snprintf(char *str, size_t size, const char *fmt, ...);

char * env_name_spec = "NAND";


#ifdef ENV_IS_EMBEDDED
extern uchar environment[];
env_t *env_ptr = (env_t *)(&environment[0]);
#else /* ! ENV_IS_EMBEDDED */
env_t *env_ptr = 0;
#endif /* ENV_IS_EMBEDDED */


/* local functions */
#if !defined(ENV_IS_EMBEDDED)
static void use_default(void);
#endif

DECLARE_GLOBAL_DATA_PTR;

uchar env_get_char_spec (int index)
{
	return ( *((uchar *)(gd->env_addr + index)) );
}


/* this is called before nand_init()
 * so we can't read Nand to validate env data.
 * Mark it OK for now. env_relocate() in env_common.c
 * will call our relocate function which will does
 * the real validation.
 *
 * When using a NAND boot image (like sequoia_nand), the environment
 * can be embedded or attached to the U-Boot image in NAND flash. This way
 * the SPL loads not only the U-Boot image from NAND but also the
 * environment.
 */
int env_init(void)
{
#if defined(ENV_IS_EMBEDDED)
	ulong total;
	int crc1_ok = 0, crc2_ok = 0;
	env_t *tmp_env1, *tmp_env2;

	total = CFG_ENV_SIZE;

	tmp_env1 = env_ptr;
	tmp_env2 = (env_t *)((ulong)env_ptr + CFG_ENV_SIZE);

	crc1_ok = (crc32(0, tmp_env1->data, ENV_SIZE) == tmp_env1->crc);
	crc2_ok = (crc32(0, tmp_env2->data, ENV_SIZE) == tmp_env2->crc);

	if (!crc1_ok && !crc2_ok)
		gd->env_valid = 0;
	else if(crc1_ok && !crc2_ok)
		gd->env_valid = 1;
	else if(!crc1_ok && crc2_ok)
		gd->env_valid = 2;
	else {
		/* both ok - check serial */
		if(tmp_env1->flags == 255 && tmp_env2->flags == 0)
			gd->env_valid = 2;
		else if(tmp_env2->flags == 255 && tmp_env1->flags == 0)
			gd->env_valid = 1;
		else if(tmp_env1->flags > tmp_env2->flags)
			gd->env_valid = 1;
		else if(tmp_env2->flags > tmp_env1->flags)
			gd->env_valid = 2;
		else /* flags are equal - almost impossible */
			gd->env_valid = 1;
	}

	if (gd->env_valid == 1)
		env_ptr = tmp_env1;
	else if (gd->env_valid == 2)
		env_ptr = tmp_env2;
#else /* ENV_IS_EMBEDDED */
	gd->env_addr  = (ulong)&default_environment[0];
	gd->env_valid = 1;
#endif /* ENV_IS_EMBEDDED */

	return (0);
}

#ifdef CMD_SAVEENV
/*
 * The legacy NAND code saved the environment in the first NAND device i.e.,
 * nand_dev_desc + 0. This is also the behaviour using the new NAND code.
 */
#ifdef CFG_ENV_OFFSET_REDUND
int saveenv(void)
{
	ulong total;
	int ret = 0;

	env_ptr->flags++;
	total = CFG_ENV_SIZE;

	if(gd->env_valid == 1) {
		puts ("Erasing redundant Nand...");
		if (nand_erase(&nand_info[0],
			       CFG_ENV_OFFSET_REDUND, CFG_ENV_SIZE))
			return 1;
		puts ("Writing to redundant Nand... ");
		ret = nand_write(&nand_info[0], CFG_ENV_OFFSET_REDUND, &total,
				 (u_char*) env_ptr);
	} else {
		puts ("Erasing Nand...");
		if (nand_erase(&nand_info[0],
			       CFG_ENV_OFFSET, CFG_ENV_SIZE))
			return 1;

		puts ("Writing to Nand... ");
		ret = nand_write(&nand_info[0], CFG_ENV_OFFSET, &total,
				 (u_char*) env_ptr);
	}
	if (ret || total != CFG_ENV_SIZE)
		return 1;

#if (!CONFIG_MSTAR_SILENT)
	puts ("done\n");
#endif
	gd->env_valid = (gd->env_valid == 2 ? 1 : 2);
	return ret;
}
#else /* ! CFG_ENV_OFFSET_REDUND */
//-----------------------------------------------------------------------------
/// Save environment to the addr pointed by offs
/// @param  offs    start address for ENV saved
/// @return   int:
/// -0: success
/// -1: nand operation encounter problem
//-----------------------------------------------------------------------------
#if  !(CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)
static int _SaveENV(ulong offs)
{
    char nand_env_write[256];
    memset(nand_env_write, 0 , 256);
    snprintf(nand_env_write, sizeof(nand_env_write), "nand_raw_write 0x%x 0x%x 0x%x 0x%x", offs, CFG_ENV_BLOCK_NUM, (void *)env_ptr, CFG_ENV_SIZE);
    return run_command(nand_env_write, 0);
}
#endif  // #if  !(CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)

int saveenv(void)
{
    int ret = 0;
    nand_info_t *nand;
#if (CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)
    char buffer[BUF_SIZE];
#endif
    nand = &nand_info[0];

#if (CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)
    memset(buffer, 0 , BUF_SIZE);
    sprintf(buffer, "ubi write %x ENV %x", (u_int)env_ptr, nand->erasesize);
    printf("\r\nenv_ptr:0x%x", (u_int)env_ptr);
    ret = run_command(buffer, 0);

#else

    /* Backup ENV before update it */
    int ret1 = _SaveENV(CFG_ENV_ADDR);
    int ret2 = _SaveENV(CFG_ENV_ADDR_BAK);
    if((0 != ret1) && (0 != ret2))//saveenv all fail
        ret = 1;
    else
        ret = 0;
#endif

    return ret;
}
#endif /* CFG_ENV_OFFSET_REDUND */
#endif /* CMD_SAVEENV */

#ifdef CFG_ENV_OFFSET_REDUND
void env_relocate_spec (void)
{
#if !defined(ENV_IS_EMBEDDED)
	ulong total;
	int crc1_ok = 0, crc2_ok = 0;
	env_t *tmp_env1, *tmp_env2;

	total = CFG_ENV_SIZE;

	tmp_env1 = (env_t *) malloc(CFG_ENV_SIZE);
	tmp_env2 = (env_t *) malloc(CFG_ENV_SIZE);

	nand_read(&nand_info[0], CFG_ENV_OFFSET, &total,
		  (u_char*) tmp_env1);
	nand_read(&nand_info[0], CFG_ENV_OFFSET_REDUND, &total,
		  (u_char*) tmp_env2);

	crc1_ok = (crc32(0, tmp_env1->data, ENV_SIZE) == tmp_env1->crc);
	crc2_ok = (crc32(0, tmp_env2->data, ENV_SIZE) == tmp_env2->crc);

	if(!crc1_ok && !crc2_ok)
		return use_default();
	else if(crc1_ok && !crc2_ok)
		gd->env_valid = 1;
	else if(!crc1_ok && crc2_ok)
		gd->env_valid = 2;
	else {
		/* both ok - check serial */
		if(tmp_env1->flags == 255 && tmp_env2->flags == 0)
			gd->env_valid = 2;
		else if(tmp_env2->flags == 255 && tmp_env1->flags == 0)
			gd->env_valid = 1;
		else if(tmp_env1->flags > tmp_env2->flags)
			gd->env_valid = 1;
		else if(tmp_env2->flags > tmp_env1->flags)
			gd->env_valid = 2;
		else /* flags are equal - almost impossible */
			gd->env_valid = 1;

	}

	free(env_ptr);
	if(gd->env_valid == 1) {
		env_ptr = tmp_env1;
		free(tmp_env2);
	} else {
		env_ptr = tmp_env2;
		free(tmp_env1);
	}

#endif /* ! ENV_IS_EMBEDDED */
}
#else /* ! CFG_ENV_OFFSET_REDUND */
//-----------------------------------------------------------------------------
/// Read environment from the addr pointed by offs
/// @param  offs    start address for ENV to be read
/// @return   int:
/// 0: success
/// 1: nand ecc correctable error
/// Negative value: nand operation encounter problem
//-----------------------------------------------------------------------------
#if  !(CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)
static int _ENV_Relocate(ulong offs)
{
#if !defined(ENV_IS_EMBEDDED)
    char nand_env_read[256];
    memset(nand_env_read, 0 , 256);
    snprintf(nand_env_read, sizeof(nand_env_read), "nand_raw_read 0x%x 0x%x 0x%x 0x%x", offs, CFG_ENV_BLOCK_NUM, (void *)env_ptr, CFG_ENV_SIZE);
    int ret = run_command(nand_env_read, 0);
    if (0 != ret)
        return ret;
    if (crc32(0, env_ptr->data, ENV_SIZE) != env_ptr->crc)
    {
        printf("Encounter read crc error.\r\n");
        return -1;
    }
    return 0;
#endif /* ! ENV_IS_EMBEDDED */
}
#endif  //#if  !(CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)

/*
 * The legacy NAND code saved the environment in the first NAND device i.e.,
 * nand_dev_desc + 0. This is also the behaviour using the new NAND code.
 */
void env_relocate_spec (void)
{
#if !defined(ENV_IS_EMBEDDED)

    nand_info_t *nand;
#if (CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)
    char buffer[BUF_SIZE];
#else
    int ret;
#endif

    nand = &nand_info[0];

#if (CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)
    memset(buffer, 0 , BUF_SIZE);
    sprintf(buffer, "ubi read %x ENV %x", (u_int)env_ptr, nand->erasesize);
    run_command(buffer, 0);
    if (crc32(0, env_ptr->data, ENV_SIZE) != env_ptr->crc)
    {
        printf("Encounter read crc error.cal_crc:0x%x, env_crc:0x%x\r\n",
                        crc32(0, env_ptr->data, ENV_SIZE), env_ptr->crc);
        return use_default();
    }

#else

    ret = _ENV_Relocate(CFG_ENV_ADDR);

       /* NAND operation encounters error */
       if ((ret < 0) && ret != -1)
       {
           *(unsigned long *)((u_char *)env_ptr + 2 * nand->oobblock
                                        - 2 * sizeof(unsigned long)) = ret;
           return use_default();
       }

       if (-1 == ret)
       {
           return use_default();
       }

       /* NAND bit flip error, rewrite ENV */
       if (1 == ret)
       {
           saveenv();
       }
#endif

#endif /* ! ENV_IS_EMBEDDED */
}
#endif /* CFG_ENV_OFFSET_REDUND */

#if !defined(ENV_IS_EMBEDDED)
static void use_default()
{
    nand_info_t *nand = &nand_info[0];
    ulong crc_tmp =  crc32(0, env_ptr->data, ENV_SIZE);

#if  !(CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)
    int result;

    //if (0xffffffff != env_ptr->crc) // on clean nand
    {
        puts ("* Warning - bad CRC or NAND, trying backup environment first\n\n");
        result = _ENV_Relocate(CFG_ENV_ADDR_BAK);

        if (result < 0)
        {
            puts ("* Warning - trying backup environment failed\n\n");

            /* Reread the error ENV for reserving */
            _ENV_Relocate(CFG_ENV_ADDR);
        }
        else  // Backup ENV can be used
        {
            /* Restore the error ENV with backup ENV */
            _SaveENV(CFG_ENV_ADDR);
            gd->env_valid = 1;
            return;
        }
    }
#endif

	puts ("*** Warning - bad CRC or NAND, using default environment\n\n");

    //default_environment_size = sizeof(default_environment);
	if (default_environment_size > CFG_ENV_SIZE)
    {
		printf ("*** Error - default environment is too large, def_size:0x%x, cfg_size:0x%x\n\n",
                        default_environment_size, CFG_ENV_SIZE);
		return;
	}

    if (0xffffffff == env_ptr->crc) // on clean nand, no error environment
    {
	    memset (env_ptr, 0, sizeof(env_t));
    }
    else // reserve error environment for the purpose of debug
    {
        *(unsigned long *)((u_char *)env_ptr + 2 * nand->oobblock
                - sizeof(unsigned long)) = crc_tmp;
        /* Indicate some reserved space is covered by default ENV */
        if (default_environment_size > nand->oobblock)
        {
            *(unsigned long *)((u_char *)env_ptr + 2 * nand->oobblock
                                              - 3 * sizeof(unsigned long)) = 0x1;
            puts ("*Warning - Some reserved space is covered by default ENV\n\n");
        }

        memcpy(((u_char *)env_ptr + nand->oobblock),
                        (u_char *)env_ptr, (nand->oobblock - 2 * sizeof(unsigned long)));
        memset (env_ptr, 0, nand->oobblock);
    }
	memcpy (env_ptr->data,
			default_environment,
			default_environment_size);
	env_ptr->crc = crc32(0, env_ptr->data, ENV_SIZE);
	gd->env_valid = 1;

}
#endif

#endif /* CFG_ENV_IS_IN_NAND */
