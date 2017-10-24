#ifndef __ASM_BUG_H__
#define __ASM_BUG_H__

#ifdef CYGPKG_INFRA_SECURITY
#define BUG() //do { diag_printf("BUG() at %s %d\n", __FILE__, __LINE__); *(int *)0=0;
#else
#define BUG() do { diag_printf("BUG() at %s %d\n", __FILE__, __LINE__); *(int *)0=0; } while (0)
#endif
#endif /* __ASM_BUG_H__ */
