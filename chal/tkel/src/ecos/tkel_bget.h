/*

   Interface definitions for bget.c, the memory management package.

*/


#ifdef __cplusplus
extern "C" {
#endif

typedef long bufsize;

void   TKEL_bpool      (void *buffer, bufsize len);
void   *TKEL_bget      (bufsize size);
void   TKEL_brel      (void *buf);
void   TKEL_bstats(bufsize* curalloc, bufsize* totfree, bufsize* maxfree, long *nget, long* nrel);
#ifdef __cplusplus
}
#endif
