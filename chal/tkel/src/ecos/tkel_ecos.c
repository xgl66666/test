#include "tkel.h"
#include "MsOS.h"
#include "drvWDT.h"
#include "tkel_ecos_private.h"

#ifdef TKEL_USE_BGET
#include "tkel_bget.h"
#endif
#ifdef TKEL_USE_DLMALLOC
#include "tkel_dlmalloc.h"
#endif

#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_intr.h>

 /*
 #define TKEL_DEBUG_MEM 
 #define TKEL_DEBUG_MEMORY_LEAK 
*/

#ifdef __DEBUG_TRACE__
#define mTKEL_DBG_PRINT(x)             \
{                            \
   printf("%s %d ", __FILE__, __LINE__);   \
   printf x ;                     \
}
#else
#define mTKEL_DBG_PRINT(x)
#endif

//#define DEBUG_HEAP_CORRUPTION

#ifdef DEBUG_HEAP_CORRUPTION
#define HEAP_MAGIC 0xA2
#endif

/*----------------------------------------------------------------------------
 *         MEMORY definitions
 *----------------------------------------------------------------------------*/
MS_U8    TKEL_Memory[TKEL_MEM_SIZE];

TKEL_int32        TKEL_poolID = -1;              /* pointer to partition      */

#ifdef TKEL_USE_BGET
static MS_S32          TKEL_bget_mutex = -1;
#endif

#ifdef TKEL_USE_DLMALLOC
static MS_S32            TKEL_dlmalloc_mutex = -1;
static mspace            TKEL_dlmalloc_mspace = NULL;
#endif


/*---------------------------------------------------------------------------------
MEMORY DEBUGGING :
   Here are defined some stuff to help debugging memory troubles.
   each allocated pointer is saved in MEM_DEBUG_ptr structure.
   each memory allocated is preceeded and followed by a tag that a task
   checks regularly. Up to 1000 pointers can be checked.
---------------------------------------------------------------------------------*/
#ifdef TKEL_DEBUG_MEM
#define DEBUG_MEM_TAG_START      (unsigned long)0x12345678
#define DEBUG_MEM_TAG_END      (unsigned long)0x87654321
typedef struct
{
   TKEL_uint8 * start;
   TKEL_msize size;
#ifdef TKEL_DEBUG_MEMORY_LEAK
   pthread_t task;
#endif
}T_DEBUG_MEM;

#define DEBUG_MEM_MAX 30000
static T_DEBUG_MEM DEBUG_MEM[DEBUG_MEM_MAX];
static TKEL_uint16 DEBUG_MEM_NB = 0;
static MS_S32   DEBUG_MEM_SEMA = NULL;
static TKEL_msize DEBUG_MEM_TOTAL_SIZE = 0;

#ifdef TKEL_DEBUG_MEMORY_LEAK
typedef struct
{
   pthread_t task;
   TKEL_uint32 nb;
   TKEL_uint32 size;
   TKEL_uint32 nb_buf[14];
}T_DEBUG_TASK_MEM;

static T_DEBUG_TASK_MEM DEBUG_TASK_MEM[100];
static TKEL_uint16 DEBUG_TASK_MEM_NB = 0;
static TKEL_msize DEBUG_MEM_MAX_SIZE = 0;
#endif
#endif
/*---------------------------------------------------------------------------------
END OF MEMORY DEBUGGING
---------------------------------------------------------------------------------*/


/*================================================================================
 *         === INITIALIZATION ===
 *================================================================================*/
TKEL_uint32 TKEL_ticksPerSecond;
TKEL_uint32 TKEL_uSecondsPerTick;
TKEL_uint32 TKEL_CPUFrequency;

/*=============================================================================
 *
 * TKEL_Init
 *
 * Parameters:
 *   size      size of memory partition
 *
 * Description:
 *      initialize kernel
 *
 * Returns:
 *      TKEL_NO_ERR   no error
 *   TKEL_BAD_ARG   wrong argument value
 *   TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_Init(void)
{
#ifdef TKEL_DEBUG_MEM
   TKEL_uint32 DEBUG_ind;
#endif
#if defined(TKEL_USE_BGET) || defined (TKEL_USE_DLMALLOC)
   void* pAddr;
#endif
   TKEL_CPUFrequency = 0 /*ST_GetClockSpeed()*/;
   TKEL_ticksPerSecond = (TKEL_uint32)sysconf(_SC_CLK_TCK);

   TKEL_poolID = MsOS_CreateMemoryPool( TKEL_MEM_SIZE,
                          0,
                          (void*)TKEL_Memory,
                          E_MSOS_FIFO,
                          "TKEL Buffer");
   mTBOX_ASSERT(TKEL_poolID > 0);

#ifdef TKEL_USE_BGET
   pAddr = MsOS_AllocateMemory((TKEL_MEM_SIZE) - 1024, TKEL_poolID);
   if (pAddr != NULL)
   {
      TKEL_bpool(pAddr, (TKEL_MEM_SIZE) - 1024);
   }
   else
   {
      printf("TKEL_Init error: MsOS_AllocateMemory(%d) failed\n", TKEL_MEM_SIZE-1024);
   }

  
   TKEL_bget_mutex =  MsOS_CreateMutex(E_MSOS_FIFO, "TKEL_BGET_MUTEX", 0);
   if (TKEL_bget_mutex < 0)
   {
       printf("TKEL_Init error: MsOS_CreateMutex(%d) failed\n", TKEL_bget_mutex); 
   }
#endif

#ifdef TKEL_USE_DLMALLOC
   pAddr = MsOS_AllocateMemory((TKEL_MEM_SIZE) - 1024, TKEL_poolID);
   TKEL_dlmalloc_mspace = create_mspace_with_base(pAddr, TKEL_MEM_SIZE-1024, 0);
   if (TKEL_dlmalloc_mspace == 0)
   {
       printf("TKEL_Init: create_mspace_with_base returned error\n");
   }
   TKEL_dlmalloc_mutex = MsOS_CreateMutex(E_MSOS_FIFO, "TKEL_DLMALLOC_MUTEX", 0);
#endif


/**-----------------------------------------------------------------------------------------------
MEMORY CHECKING DEBUG PART : When DEBUG is defined, a task is created to check all pointers.
all allocated memory is preceeded and followed by a specific tag, that a task checks regularly.
This task only warns that trouble has happened, and tells where.
Each allocation uses from 8 to 11 bytes more that asked.
An array counts the number of allocations done for sizes of 2^n to 2^(n+1) (n is from 0 to 15)
-----------------------------------------------------------------------------------------------**/
#ifdef TKEL_DEBUG_MEM
   DEBUG_MEM_SEMA = MsOS_CreateMutex(E_MSOS_FIFO, "TKEL_MEM_DEBUG_MUTEX", 0);

   for (DEBUG_ind=0; DEBUG_ind<DEBUG_MEM_MAX; DEBUG_ind++) {
      DEBUG_MEM[DEBUG_ind].start = 0;
      DEBUG_MEM[DEBUG_ind].size = 0;
#ifdef TKEL_DEBUG_MEMORY_LEAK
      DEBUG_MEM[DEBUG_ind].task = NULL;
#endif
}

#ifdef TKEL_DEBUG_MEMORY_LEAK
   for (DEBUG_ind=0; DEBUG_ind<100; DEBUG_ind++) {
      DEBUG_TASK_MEM[DEBUG_ind].task = NULL;
      DEBUG_TASK_MEM[DEBUG_ind].nb = 0;
      DEBUG_TASK_MEM[DEBUG_ind].size = 0;
   }
#endif
#endif
/**-----------------------------------------------------------------------------------------------
END OF INITIALIZATION OF MEMORY CHECKING DEBUG PART
-----------------------------------------------------------------------------------------------**/


   return TKEL_NO_ERR;
}

/*=============================================================================
 *
 * TKEL_GetMemoryPartition
 *
 * Parameters:
 *      None
 *
 * Description:
 *      This function returns value of pointer used by allocater
 *
 * Returns:
 *      NULL if no partition exists
 *      else
 *      void * partition
 *
 *===========================================================================*/
void* TKEL_GetMemoryPartition(void)
{
   return (void*) NULL;
}

/*=============================================================================
 *
 * TKEL_GetTick
 *
 * Parameters:
 *      pTicks      number of ticks elapsed since last reset
 *
 * Description:
 *      Retreive the number of ticks elapsed since last reset
 *
 * Returns:
 *      TKEL_NO_ERR      no error
 *      TKEL_BAD_ARG   wrong argument value
 *      TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_GetTick(TKEL_tck *pTick)
{
	if (pTick == NULL)
	{
		return(TKEL_BAD_ARG);
	}
#if defined (MSOS_TYPE_UCOS) || defined (MSOS_TYPE_ECOS)
	*pTick = MsOS_GetSystemTime();
#else
	struct timespec ts;
	(void) clock_gettime(/*CLOCK_REALTIME*/, &ts);

	/* ignore overflow */
	/* this conversion has milliseconds sensitivity, so it is biased to milliseconds */
	*pTick = ts.tv_sec * TKEL_ticksPerSecond + ((ts.tv_nsec / 1000000) * TKEL_ticksPerSecond) / 1000;
#endif
	return(TKEL_NO_ERR);
}


/*=============================================================================
 *
 * TKEL_Term
 *
 * Parameters:
 *
 *
 * Description:
 *       termination kernel
 * Returns:
 *      TKEL_NO_ERR   no error
 *
 *===========================================================================*/
TKEL_err TKEL_Term(void)
{
#ifdef TKEL_USE_BGET
	MsOS_DeleteMutex(TKEL_bget_mutex);
#endif

#ifdef TKEL_USE_DLMALLOC
	MsOS_DeleteMutex(TKEL_dlmalloc_mutex);
#endif
	MsOS_DeleteMemoryPool(TKEL_poolID);    
	TKEL_poolID = -1;

#ifdef TKEL_DEBUG_MEM
	MsOS_DeleteMutex(DEBUG_MEM_SEMA);
#endif
	return TKEL_NO_ERR;
}

/*================================================================================*
 *          Memory management
 *================================================================================*/

/*=============================================================================
 *
 * TKEL_Malloc
 *
 * Parameters:
 *   memorySize      Size of the memory buffer
 *   buffer       Points to the starting address of the memory buffer
 *
 * Description:
 *      Allocates a memory buffer
 *
 * Returns:
 *      TKEL_NO_ERR   no error
 *   TKEL_BAD_ARG   wrong argument value
 *   TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_Malloc (TKEL_msize memorySize, void **buffer)
{
#ifdef TKEL_DEBUG_MEM
	TKEL_uint8 * DEBUG_buffer;
	pthread_t *task;
	TKEL_task_id TmpTask;
	static TKEL_uint8 DEBUG_MEM_NB_MALLOC=0;
	static TKEL_uint8 dummyTest = 0;

	if(buffer == NULL)
	{
		return(TKEL_BAD_ARG);
	}

	MsOS_ObtainMutex(DEBUG_MEM_SEMA, MSOS_WAIT_FOREVER);

#ifdef TKEL_DEBUG_MEMORY_LEAK
	if ((memorySize > 8 && memorySize <= 12) && (pthread_self() == (pthread_t*)0xc03f5550))
	{
		dummyTest++;
	}
#endif

	if (DEBUG_MEM_NB>=DEBUG_MEM_MAX)
	{
		printf("!!!!!!!!!!!!!!!!!! T0O MANY ALLOCATIONS !!!!!!!!!!!!!!!!!!\n");
		MsOS_ReleaseMutex(DEBUG_MEM_SEMA);
		return TKEL_NOT_DONE;
	}
	memorySize += 8;       /* add space for tags */
#endif

#ifdef TKEL_USE_BGET
	if (MsOS_ObtainMutex(TKEL_bget_mutex, MSOS_WAIT_FOREVER) == TRUE)
	{
#ifdef DEBUG_HEAP_CORRUPTION
		uint8_t *p = TKEL_bget(memorySize + 5);

		if (p != NULL)
		{
			p[0] = HEAP_MAGIC;
			p[3] = memorySize & 0xFF;
			p[2] = (memorySize >> 8) & 0xFF;
			p[1] = (memorySize >> 16) & 0xFF;

			p[memorySize + 4] = HEAP_MAGIC;

			*buffer = p + 4;
		}
		else
		{
			*buffer = NULL;
		}
#else
		*buffer = TKEL_bget(memorySize);
		if(*buffer == NULL)
		{
			printf("[###%s###]TKEL_bget 0x%x failed.TKEL_poolID=0x%x,buffer=%p,line=%d\n",__FUNCTION__,memorySize,TKEL_poolID,*buffer,__LINE__);
		}
#endif
		MsOS_ReleaseMutex(TKEL_bget_mutex);
	}
	else
	{
		return TKEL_NOT_DONE;
	}
   
#else
#ifdef TKEL_USE_DLMALLOC
	if (MsOS_ObtainMutex(TKEL_dlmalloc_mutex, MSOS_WAIT_FOREVER) == TRUE)
	{
		*buffer = mspace_malloc(TKEL_dlmalloc_mspace, memorySize);
		MsOS_ReleaseMutex(TKEL_dlmalloc_mutex);
	}
	else
	{
		return TKEL_NOT_DONE;
	}

#else
	*buffer = MsOS_AllocateMemory((size_t)memorySize, TKEL_poolID);
#endif
#endif

	if (*buffer)
	{
#ifdef TKEL_DEBUG_MEM
		DEBUG_buffer = (TKEL_uint8*) *buffer;
		DEBUG_buffer[0] = (TKEL_uint8) ((DEBUG_MEM_TAG_START >> 24) & 0xFF);
		DEBUG_buffer[1] = (TKEL_uint8) ((DEBUG_MEM_TAG_START >> 16) & 0xFF);
		DEBUG_buffer[2] = (TKEL_uint8) ((DEBUG_MEM_TAG_START >> 8) & 0xFF);
		DEBUG_buffer[3] = (TKEL_uint8) (DEBUG_MEM_TAG_START & 0xFF);
		DEBUG_buffer[memorySize-4] = (TKEL_uint8) ((DEBUG_MEM_TAG_END >> 24) & 0xFF);
		DEBUG_buffer[memorySize-3] = (TKEL_uint8) ((DEBUG_MEM_TAG_END >> 16) & 0xFF);
		DEBUG_buffer[memorySize-2] = (TKEL_uint8) ((DEBUG_MEM_TAG_END >> 8) & 0xFF);
		DEBUG_buffer[memorySize-1] = (TKEL_uint8) (DEBUG_MEM_TAG_END & 0xFF);
		DEBUG_MEM[DEBUG_MEM_NB].start = DEBUG_buffer;
		DEBUG_MEM[DEBUG_MEM_NB].size = memorySize;
#ifdef TKEL_DEBUG_MEMORY_LEAK
		DEBUG_MEM[DEBUG_MEM_NB].task = pthread_self();
#endif
		DEBUG_MEM_NB++;
		DEBUG_MEM_TOTAL_SIZE += memorySize;
		DEBUG_MEM_NB_MALLOC++;
#ifdef TKEL_DEBUG_MEMORY_LEAK
		if (DEBUG_MEM_TOTAL_SIZE > DEBUG_MEM_MAX_SIZE)
		{
			DEBUG_MEM_MAX_SIZE = DEBUG_MEM_TOTAL_SIZE;
			if (DEBUG_MEM_NB_MALLOC > 100)
			{
				printf("MALLOC : %lu\n",DEBUG_MEM_TOTAL_SIZE);
				DEBUG_MEM_NB_MALLOC = 0;
			}
		}
#else
		if (DEBUG_MEM_NB_MALLOC==25)
		{
			DEBUG_MEM_NB_MALLOC = 0;
			task = pthread_self();
			TmpTask = GetTaskFromList(task);
			printf("MALLOC : task=[%p] ptr=[%p] total=%lu\n",(void*)TmpTask,*buffer,DEBUG_MEM_TOTAL_SIZE);
		}
#endif
		*buffer = DEBUG_buffer + 4;
		MsOS_ReleaseMutex(DEBUG_MEM_SEMA);
#endif   
		return TKEL_NO_ERR;
	}
	else
	{
#if defined(TKEL_USE_BGET) || defined(TKEL_USE_DLMALLOC)
		printf("Alloc Failed size : %d\n", (int)memorySize);
#else    
		void *PoolAddr;
		TKEL_uint32 u32PoolSize;
		TKEL_uint32 u32FreeSize;
		TKEL_uint32 u32LargestFreeBlockSize;

		printf("Alloc Failed size : %d\n", (int)memorySize);

		if(MsOS_InfoMemoryPool(TKEL_poolID, &PoolAddr, &u32PoolSize, &u32FreeSize, &u32LargestFreeBlockSize) == FALSE)
		{
			printf("!!!!!!!!!!!!!MsOS_InfoMemoryPool error   TKEL_poolID\n");
			return TKEL_NOT_DONE;
		}
		printf("TKEL_poolID startAddr=0x%x size=%d largest=%d free=%d\n", PoolAddr, u32PoolSize, u32LargestFreeBlockSize, u32FreeSize);
#endif
#ifdef TKEL_DEBUG_MEM
		printf("!!!!!!!!!!!!!!!!! MALLOC : NO MORE MEMORY !!!!!!!!!!!!!!!!!\n");
		MsOS_ReleaseMutex(DEBUG_MEM_SEMA);
#endif
		return TKEL_NOT_DONE;
	}
}


#ifdef TKEL_DEBUG_MEMORY_LEAK
/* Cette fonction permet de d?cter des fuites m?ire */
/* Pour cela, il faut appeler cette fonction periodiquement (sur un appuie touche ou dans un zapping auto) */

/* les traces donnent les infos suivantes */
/* - task_id et nom de la tache (comprenant l'addr du point d'entr?de la tache) qui a fait des allocs TKEL */
/* - nb et taille totale des alloc TKEL pour chacune des taches */
/* - r?rtition des allocs en fonction de leur taille (souvent, les fuites memoire concernent des buffers de taille identique) */

/* 1- d?rminer la tache et la taille des buffers non lib?s */
/* 2- affiner les seuils utilis?pour classer les buffers par tailles afin de d?rminer au plus juste la taille des buffers non lib?s */
/* 3- utiliser ces seuils dans TKEL_Malloc (test dummyTest++) ainsi que le task_id de la tache qui fuit */
/*   (d'une compil a l'autre, la valeur du task_id ne change pas si aucune autre modif dans le code n'est faite) */
/* 4- mettre un breakpoint sur dummyTest++ */

#define kDBG_MEM_THRESHOLD_0   (16   + 8)
#define kDBG_MEM_THRESHOLD_1   (32   + 8)
#define kDBG_MEM_THRESHOLD_2   (64   + 8)
#define kDBG_MEM_THRESHOLD_3   (128   + 8)
#define kDBG_MEM_THRESHOLD_4   (256   + 8)
#define kDBG_MEM_THRESHOLD_5   (512   + 8)
#define kDBG_MEM_THRESHOLD_6   (1024   + 8)
#define kDBG_MEM_THRESHOLD_7   (2048   + 8)
#define kDBG_MEM_THRESHOLD_8   (4096   + 8)
#define kDBG_MEM_THRESHOLD_9   (8192   + 8)
#define kDBG_MEM_THRESHOLD_10 (16384 + 8)
#define kDBG_MEM_THRESHOLD_11 (32768 + 8)
#define kDBG_MEM_THRESHOLD_12 (65536 + 8)

void TKEL_DebugPrintMemoryUsage(void)
{
   int i, j;
   TKEL_bool found;

   printf("\n\n\n\n ############################## TKEL_DebugPrintMemoryUsage\n");

   MsOS_ObtainMutex(DEBUG_MEM_SEMA, MSOS_WAIT_FOREVER);
   
   for (j=0 ; j<DEBUG_TASK_MEM_NB ; j++)
   {
      DEBUG_TASK_MEM[j].task = NULL;
      DEBUG_TASK_MEM[j].nb = 0;
      DEBUG_TASK_MEM[j].size = 0;
   }
   DEBUG_TASK_MEM_NB = 0;

   for (i=0 ; i<DEBUG_MEM_NB ; i++)
   {
      if (DEBUG_MEM[i].task != NULL)
      {
         found = FALSE;
         for (j=0 ; j<DEBUG_TASK_MEM_NB ; j++)
         {
          if (DEBUG_MEM[i].task == DEBUG_TASK_MEM[j].task)
          {
              found = TRUE;
              DEBUG_TASK_MEM[j].nb++;
              DEBUG_TASK_MEM[j].size += DEBUG_MEM[i].size;
              
          #if 1
              /* display the number of buffers */
              if (DEBUG_MEM[i].size      <= kDBG_MEM_THRESHOLD_0 )   {DEBUG_TASK_MEM[j].nb_buf[0]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_1 )   {DEBUG_TASK_MEM[j].nb_buf[1]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_2 )   {DEBUG_TASK_MEM[j].nb_buf[2]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_3 )   {DEBUG_TASK_MEM[j].nb_buf[3]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_4 )   {DEBUG_TASK_MEM[j].nb_buf[4]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_5 )   {DEBUG_TASK_MEM[j].nb_buf[5]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_6 )   {DEBUG_TASK_MEM[j].nb_buf[6]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_7 )   {DEBUG_TASK_MEM[j].nb_buf[7]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_8 )   {DEBUG_TASK_MEM[j].nb_buf[8]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_9 )   {DEBUG_TASK_MEM[j].nb_buf[9]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_10)   {DEBUG_TASK_MEM[j].nb_buf[10]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_11)   {DEBUG_TASK_MEM[j].nb_buf[11]++;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_12)   {DEBUG_TASK_MEM[j].nb_buf[12]++;}
              else                                 {DEBUG_TASK_MEM[j].nb_buf[13]++;}
          #else
              /* display the total size of buffers */
              if (DEBUG_MEM[i].size      <= kDBG_MEM_THRESHOLD_0 )   {DEBUG_TASK_MEM[j].nb_buf[0]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_1 )   {DEBUG_TASK_MEM[j].nb_buf[1]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_2 )   {DEBUG_TASK_MEM[j].nb_buf[2]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_3 )   {DEBUG_TASK_MEM[j].nb_buf[3]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_4 )   {DEBUG_TASK_MEM[j].nb_buf[4]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_5 )   {DEBUG_TASK_MEM[j].nb_buf[5]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_6 )   {DEBUG_TASK_MEM[j].nb_buf[6]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_7 )   {DEBUG_TASK_MEM[j].nb_buf[7]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_8 )   {DEBUG_TASK_MEM[j].nb_buf[8]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_9 )   {DEBUG_TASK_MEM[j].nb_buf[9]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_10)   {DEBUG_TASK_MEM[j].nb_buf[10]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_11)   {DEBUG_TASK_MEM[j].nb_buf[11]+= DEBUG_MEM[i].size;}
              else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_12)   {DEBUG_TASK_MEM[j].nb_buf[12]+= DEBUG_MEM[i].size;}
              else                                 {DEBUG_TASK_MEM[j].nb_buf[13]+= DEBUG_MEM[i].size;}
          #endif
              break;
          }
         }
         
         if (found == FALSE)
         {
          DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].task = DEBUG_MEM[i].task;
          DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb = 1;
          DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].size = DEBUG_MEM[i].size;
          
          memset(DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf, 0, sizeof(DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf));

         #if 1
          /* display the number of buffers */
          if (DEBUG_MEM[i].size      <= kDBG_MEM_THRESHOLD_0 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[0] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_1 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[1] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_2 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[2] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_3 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[3] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_4 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[4] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_5 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[5] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_6 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[6] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_7 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[7] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_8 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[8] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_9 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[9] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_10)   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[10] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_11)   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[11] = 1;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_12)   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[12] = 1;}
          else                                 {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[13] = 1;}
         #else
          /* display the total size of buffers */
          if (DEBUG_MEM[i].size      <= kDBG_MEM_THRESHOLD_0 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[0] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_1 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[1] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_2 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[2] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_3 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[3] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_4 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[4] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_5 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[5] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_6 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[6] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_7 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[7] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_8 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[8] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_9 )   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[9] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_10)   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[10] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_11)   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[11] = DEBUG_MEM[i].size;}
          else if (DEBUG_MEM[i].size <= kDBG_MEM_THRESHOLD_12)   {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[12] = DEBUG_MEM[i].size;}
          else                                 {DEBUG_TASK_MEM[DEBUG_TASK_MEM_NB].nb_buf[13] = DEBUG_MEM[i].size;}
         #endif
          DEBUG_TASK_MEM_NB++;
         }
      }
   }
   
   
   for (j=0 ; j<DEBUG_TASK_MEM_NB ; j++)
   {
        printf("\############################## 0x%x , %d, %d [%d %d %d %d %d %d %d %d %d %d %d %d %d %d]\n", 
         DEBUG_TASK_MEM[j].task,
       //  task_name(DEBUG_TASK_MEM[j].task), 
         DEBUG_TASK_MEM[j].nb, 
         DEBUG_TASK_MEM[j].size,
         DEBUG_TASK_MEM[j].nb_buf[0],
         DEBUG_TASK_MEM[j].nb_buf[1],
         DEBUG_TASK_MEM[j].nb_buf[2],
         DEBUG_TASK_MEM[j].nb_buf[3],
         DEBUG_TASK_MEM[j].nb_buf[4],
         DEBUG_TASK_MEM[j].nb_buf[5],
         DEBUG_TASK_MEM[j].nb_buf[6],
         DEBUG_TASK_MEM[j].nb_buf[7],
         DEBUG_TASK_MEM[j].nb_buf[8],
         DEBUG_TASK_MEM[j].nb_buf[9],
         DEBUG_TASK_MEM[j].nb_buf[10],
         DEBUG_TASK_MEM[j].nb_buf[11],
         DEBUG_TASK_MEM[j].nb_buf[12],
         DEBUG_TASK_MEM[j].nb_buf[13]);
   }

   MsOS_ReleaseMutex(DEBUG_MEM_SEMA);
}   
#endif


/*=============================================================================
 *
 * TKEL_PartitionStatus
 *
 * Parameters:
 *   none
 *
 * Description:
 *      Display partition status
 *
 * Returns:
 *   nothing
 *
 *===========================================================================*/
void TKEL_PartitionStatus(void)
{
#ifdef TKEL_USE_BGET
#if 1 /* to get stats, flag "BufStats" needs to be enabled in tkel_bget.c */
	bufsize curalloc = 0;
	bufsize totfree = 0;
	bufsize maxfree = 0;
	long nget = 0;
	long nrel = 0;

	MsOS_ObtainMutex(TKEL_bget_mutex, MSOS_WAIT_FOREVER);
	TKEL_bstats(&curalloc, &totfree, &maxfree, &nget, &nrel);
	MsOS_ReleaseMutex(TKEL_bget_mutex);
	//printf("[%s]size=0x%x largest=0x%x free=0x%x used=0x%x\n",__FUNCTION__,curalloc+totfree, maxfree, totfree,curalloc);
	printf("[TKEL_MEM]\n");
	printf("size\t\t%x\n", curalloc+totfree);
	printf("free\t\t%x\n", totfree);
	printf("largest\t\t%x\n", maxfree);
	printf("used\t\t%x\n", curalloc);
#else
	printf("to get stats, flag 'BufStats' needs to be enabled in tkel_bget.c\n");
#endif

#else

#ifdef TKEL_USE_DLMALLOC
	/* TODO */
#if !NO_MALLOC_STATS
	mspace_malloc_stats(TKEL_dlmalloc_mspace);
#endif
#endif

#endif
}



/*=============================================================================
 *
 * TKEL_Free
 *
 * Parameters:
 *   buffer       Starting address of the memory buffer
 *
 * Description:
 *      Releases a memory buffer
 *
 * Returns:
 *      TKEL_NO_ERR   no error
 *   TKEL_BAD_ARG   wrong argument value
 *   TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_Free(void *buffer)
{
#ifdef TKEL_DEBUG_MEM
	TKEL_uint8 * DEBUG_buffer;
	TKEL_int32 DEBUG_ind;
	TKEL_msize memosize = 0;
	pthread_t *task;
	TKEL_task_id TmpTask;
	static TKEL_uint8 DEBUG_MEM_NB_FREE=0;

	MsOS_ObtainMutex(DEBUG_MEM_SEMA, MSOS_WAIT_FOREVER);

	DEBUG_buffer = (TKEL_uint8*) buffer - 4;
	for (DEBUG_ind=(TKEL_int32) DEBUG_MEM_NB-1; DEBUG_ind>=0; DEBUG_ind--)
	{
		if (DEBUG_MEM[DEBUG_ind].start == DEBUG_buffer)
		{
			break;
		}
	}

	if (DEBUG_ind < 0)
	{
		printf ("!!!!!!!! FREE : pointer not found !!!!!!!!\n");
		MsOS_ReleaseMutex(DEBUG_MEM_SEMA);
		return TKEL_NOT_DONE;
	}

	memosize = DEBUG_MEM[DEBUG_ind].size;
	if (DEBUG_buffer[0] != (TKEL_uint8) ((DEBUG_MEM_TAG_START >> 24) & 0xFF) ||
      DEBUG_buffer[1] != (TKEL_uint8) ((DEBUG_MEM_TAG_START >> 16) & 0xFF) ||
      DEBUG_buffer[2] != (TKEL_uint8) ((DEBUG_MEM_TAG_START >> 8) & 0xFF) ||
      DEBUG_buffer[3] != (TKEL_uint8) (DEBUG_MEM_TAG_START & 0xFF))
	{
		printf("!!!!!!!! FREE : START TAG has been corrupted !!!!!!!!\n");
		MsOS_ReleaseMutex(DEBUG_MEM_SEMA);
		return TKEL_NOT_DONE;
	}

	if (DEBUG_buffer[memosize-4] != (TKEL_uint8) ((DEBUG_MEM_TAG_END >> 24) & 0xFF) ||
      DEBUG_buffer[memosize-3] != (TKEL_uint8) ((DEBUG_MEM_TAG_END >> 16) & 0xFF) ||
      DEBUG_buffer[memosize-2] != (TKEL_uint8) ((DEBUG_MEM_TAG_END >> 8) & 0xFF) ||
      DEBUG_buffer[memosize-1] != (TKEL_uint8) (DEBUG_MEM_TAG_END & 0xFF))
	{
		printf("!!!!!!!! FREE : END TAG has been corrupted !!!!!!!!\n");
		MsOS_ReleaseMutex(DEBUG_MEM_SEMA);
		return TKEL_NOT_DONE;
	}
	buffer = (void*) DEBUG_buffer;
#endif

	if(buffer == NULL)
	{
		mTKEL_DBG_PRINT(("TKEL_Free: WARNING - Deallocation of NULL pointer\n"));
		return TKEL_BAD_ARG;
	}

#ifdef TKEL_USE_BGET
	if (MsOS_ObtainMutex(TKEL_bget_mutex, MSOS_WAIT_FOREVER) == TRUE)
	{
#ifdef DEBUG_HEAP_CORRUPTION
		uint8_t * q = buffer - 4;
		uint32_t size;

		if (q[0] != HEAP_MAGIC)
		{
			printf("Memory overwritten at front\n");
			int * r = 0;
			*r = 0;
		}

		size = ((q[1] << 16) & 0xFF0000) | ((q[2] << 8) & 0xFF00) | (q[3] & 0xFF);

		if (q[size + 4] != HEAP_MAGIC)
		{
			printf("Memory overwritten at end, size = %d\n", size);
			int * r = 0;
			*r = 0;
		}

		buffer = q;
#endif

		TKEL_brel(buffer);
		//printf("[%s  ]TKEL_poolID=0x%x,pstart=%p,line=%d\n",__FUNCTION__,TKEL_poolID,buffer,__LINE__);

		MsOS_ReleaseMutex(TKEL_bget_mutex);
	}
#else
#ifdef TKEL_USE_DLMALLOC
	if (buffer != NULL)
	{
		if (MsOS_ObtainMutex(TKEL_dlmalloc_mutex, MSOS_WAIT_FOREVER) == TRUE)
		{
			mspace_free(TKEL_dlmalloc_mspace, buffer);
			MsOS_ReleaseMutex(TKEL_dlmalloc_mutex);
		}
		else
		{
			return TKEL_NOT_DONE;
		}
	}
#else
    MsOS_FreeMemory(TKEL_poolID, buffer);
	printf("[%s  ]TKEL_poolID=0x%x,pstart=%p,line=%d\n",__FUNCTION__,TKEL_poolID,buffer,__LINE__);
#endif
#endif

#ifdef TKEL_DEBUG_MEM
	DEBUG_MEM_TOTAL_SIZE -= DEBUG_MEM[DEBUG_ind].size;
	DEBUG_MEM_NB_FREE++;
#ifndef TKEL_DEBUG_MEMORY_LEAK
	if (DEBUG_MEM_NB_FREE==25)
	{
		DEBUG_MEM_NB_FREE = 0;
		task = pthread_self();
		TmpTask = GetTaskFromList(task);
		printf("FREE : task=[%p] ptr=[%p] total=%lu\n",(void*) TmpTask,buffer,DEBUG_MEM_TOTAL_SIZE);
	}
#endif
	for (; DEBUG_ind<((TKEL_int32)DEBUG_MEM_NB-1); DEBUG_ind++)
	{
		DEBUG_MEM[DEBUG_ind].start = DEBUG_MEM[DEBUG_ind+1].start ;
		DEBUG_MEM[DEBUG_ind].size = DEBUG_MEM[DEBUG_ind+1].size;
#ifdef TKEL_DEBUG_MEMORY_LEAK
		DEBUG_MEM[DEBUG_ind].task = DEBUG_MEM[DEBUG_ind+1].task;
#endif
	}
	DEBUG_MEM_NB--;
	MsOS_ReleaseMutex(DEBUG_MEM_SEMA);
#endif

	return TKEL_NO_ERR;
}


/*================================================================================*
 *          ISR Management
 *================================================================================*/

/*=============================================================================
 *
 * TKEL_EnterIT
 *
 * Parameters:
 *   frame      Stack frame
 *
 * Description:
 *      Enter an Interrupt Service Routine
 *
 * Returns:
 *
 *===========================================================================*/
TKEL_frame TKEL_EnterIT(TKEL_frame frame)
{
   return TKEL_NO_ERR;
}

/*=============================================================================
 *
 * TKEL_ExitIT
 *
 * Parameters:
 *   frame      Stack frame
 *
 * Description:
 *      Exit the current Interrupt Service Routine
 *
 * Returns:
 *
 *===========================================================================*/
TKEL_frame TKEL_ExitIT(TKEL_frame frame)
{
   return TKEL_NO_ERR;
}


/*================================================================================*
 *          Miscellaneous
 *================================================================================*/

/*=============================================================================
 *
 * TKEL_Abort
 *
 * Parameters:
 *   cause      cause of the abort request
 *
 * Description:
 *      Aborts an enters fatal error handling mode.
 *      Never returns.
 *
 * Returns:
 *
 *===========================================================================*/
void TKEL_Abort(TKEL_cause cause)
{
   //abort();
    MDrv_WDT_Init(E_WDT_DBGLV_ALL);
    MDrv_WDT_SetTimer(E_WDT_DBGLV_ALL, 1);

    MsOS_DelayTask(2000);


}

/******************************************************************************
 * Function Name : __wrap_cyg_hal_exception_handler
 *
 * Description   : This is a wrapper around cyg_hal_exception_handler() made to
 *                 avoid occasional crashes in backtrace_mips32() in eCos (a
 *                 function called by stack_dump() which is called by
 *                 cyg_hal_exception_handler()). This function is enabled with
 *                 flag "--wrap cyg_hal_exception_handler" sent to linker. This
 *                 function calls cyg_hal_deliver_exception() directly.
 *
 *                 If it becomes possible to use cyg_hal_exception_handler()
 *                 normally, remove this function.
 *
 * Side effects  : none
 *
 * Comment       :
 *
 * Parameters    : regs - as per definition found on Internet
 *
 * Returns       : 0 - as per definition found on Internet
 *
 /*****************************************************************************/
cyg_uint32 __wrap_cyg_hal_exception_handler(HAL_SavedRegisters *regs){
    int vec = regs->vector>>2;
    cyg_hal_deliver_exception( vec, (CYG_ADDRWORD)regs );
    return 0;
}

IMPORT void TDAL_Reset(void);

/* Function given in TKEL_SetExceptionHandler to be called during exception handling */
LOCAL void (*TKELi_exceptionFunction)(void) = NULL;

LOCAL tTKEL_ExceptionInfo TKELi_exceptionInfo;
LOCAL CYG_HAL_MIPS_REG TKELi_breakAddress;

/******************************************************************************
 * Function Name : _TKEL_EcosCustomExceptionHandler
 *
 * Description   : This function is used as the exception handler in eCos. This
 *                 function is set to be called by cyg_hal_deliver_exception()
 *                 after an exception happens if the user desires to analyze
 *                 the exception by sending the string, otherwise resets the
 *                 system.
 *
 * Side effects  : After this function ends, exception handling will finish and
 *                 _DBGDIAG_AfterException() will be executed. R31 (register
 *                 used for return address on MIPS) is set to start of
 *                 TDAL_Reset().
 *
 * Comment       :
 *
 * Parameters    : data - pointer to address where exception info address needs
 *                 to be saved
 *                 exception_number - exception number sent by
 *                 cyg_hal_deliver_exception()
 *                 info - pointer to HAL_SavedRegisters
 *
 * Returns       : nothing
 *
 /*****************************************************************************/
LOCAL void TKELi_EcosCustomExceptionHandler(
    cyg_addrword_t data,
    cyg_code_t   exception_number,
    cyg_addrword_t info
){
    // Enable output from TBOX_Print for this message in case exception is encountered during handling
    TBOX_UnblockAccess();
    mTBOX_PRINT((kTBOX_LF, "*** System crashed, error code %d ***\n", exception_number));
    HAL_SavedRegisters *regs = (HAL_SavedRegisters*)info;

    TKELi_exceptionInfo.savedRegsCount = 34;
    int i;
    for(i = 0; i < 32; i++)
    {
        snprintf(TKELi_exceptionInfo.savedRegs[i].name, 6, "%sR%d", i<=9?" ":"", i);
        TKELi_exceptionInfo.savedRegs[i].value = regs->d[i];
    }
    sprintf(TKELi_exceptionInfo.savedRegs[32].name, " HI");
    TKELi_exceptionInfo.savedRegs[32].value = regs->hi;
    sprintf(TKELi_exceptionInfo.savedRegs[33].name, " LO");
    TKELi_exceptionInfo.savedRegs[33].value = regs->lo;

    TKELi_breakAddress = regs->pc;
    // If a function is set, call it afterwards, otherwise go to reset immediately
    if(TKELi_exceptionFunction != NULL)
    {
        regs->pc = (int)TKELi_exceptionFunction;
        regs->d[31] = (int)&TDAL_Reset;
    }
    else
    {
        regs->pc = (int)&TDAL_Reset;
    }
    // data is pointer to address where exception info address needs to be saved
    if(data != NULL)
    {
        *((tTKEL_ExceptionInfo**)data) = &TKELi_exceptionInfo;
    }
}

/*=============================================================================
 *
 * TKEL_SetExceptionHandler
 *
 * Parameters:
 *   function               function to be called
 *   exceptionInfoLocation  location where address of saved exception info
 *                          will be saved after exception handling or NULL
 *
 * Description:
 *      Sets the given function to be called during exception handling. After
 *      exception handler finishes, caller will get address of location where
 *      exception info is stored in exceptionInfoPointer. If the function is
 *      NULL, default handling will be set instead. If exceptionInfoPointer is
 *      NULL, exception info location will not be given to caller.
 *
 * Returns:
 *   TKEL_NO_ERR   no error
 *   TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_SetExceptionHandler(void (*function)(void), tTKEL_ExceptionInfo** exceptionInfoLocation)
{
    int i;
    TKELi_exceptionFunction = function;
    if(function == NULL)
    {
        for(i = CYGNUM_HAL_VSR_MIN; i <= CYGNUM_HAL_VSR_MAX; i++)
        {
            cyg_exception_clear_handler(i);
        }
    }
    else
    {
        for(i = CYGNUM_HAL_VSR_MIN; i <= CYGNUM_HAL_VSR_MAX; i++)
        {
            cyg_exception_set_handler(i, TKELi_EcosCustomExceptionHandler, (cyg_addrword_t)exceptionInfoLocation, NULL, NULL);
        }
    }
    return TKEL_NO_ERR;
}

#ifndef USE_TKEL_MEMORY_PARTITIONS
#define USE_TKEL_MEMORY_PARTITIONS
#endif

#ifdef USE_TKEL_MEMORY_PARTITIONS
/* ======================================================================== */
/*      TKEL Memory Partition Allocation functions             */
/* ======================================================================== */

/*=============================================================================
 *
 * TKEL_CreateFixedSizeMemPart
 *
 * Parameters:
 *   bufferSize      size of buffers
 *   memPartSize      count of buffers
 *   memPartAddress   address of the buffer pool
 *   memPartID       memory partition ID
 *
 * Description:
 *      Creates a memory partition
 *
 * Returns:
 *   TKEL_NO_ERR   no error
 *   TKEL_BAD_ARG   wrong argument value
 *   TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_CreateFixedSizeMemPart(
          TKEL_uint32      bufferSize,
          TKEL_uint32      memPartSize,
          void*          memPartAddress,
          TKEL_memPart_id   *memPartID)
{
	TKEL_fixedSizeMemPart   *pMemPart = NULL;
	TKEL_uint8          *pBuffer = NULL;
	TKEL_uint8         **ppNextBuffer = NULL;
	TKEL_uint32          i, count;

	/* check parameters */
	if ((bufferSize < sizeof(TKEL_uint8*)) ||
      ((bufferSize & 3) != 0) ||
      (memPartSize < bufferSize) ||
      (memPartAddress == NULL) ||
      (((int)memPartAddress & 3) != 0 ) ||
      (memPartID == NULL))
	{
		/*
		* bufferSize is less than pointer size
		* or bufferSize not aligned on size of a pointer
		* or memPartSize is less than the size of one buffer
		* or memPartAddress is null
		* or memPartAddress not aligned on size of a pointer
		* or memPartID is null
		*/
		mTKEL_DBG_PRINT(("TKEL_CreateFixedSizeMemPart: ERROR\n"));
		return TKEL_BAD_ARG;
	}

	/* allocate memory partition descriptor */
	if (TKEL_Malloc(sizeof(TKEL_fixedSizeMemPart), (void **)&pMemPart) != TKEL_NO_ERR)
	{
		mTKEL_DBG_PRINT(("TKEL_CreateFixedSizeMemPart: ERROR\n"));
		return TKEL_NOT_DONE;
	}

	if (pMemPart == NULL)
	{
		mTKEL_DBG_PRINT(("TKEL_Malloc pMemPart failed\n"));
		return TKEL_NOT_DONE;
	}
   
	memset(pMemPart, 0, sizeof(TKEL_fixedSizeMemPart));

	/* allocate memory partition semaphore */
	if (TKEL_CreateSemaphore(1, &(pMemPart->lock)) != TKEL_NO_ERR)
	{
		TKEL_Free(pMemPart);
		mTKEL_DBG_PRINT(("TKEL_CreateFixedSizeMemPart: ERROR\n"));
		return TKEL_NOT_DONE;
	}

	/* initialize the memory partition descriptor */
	pMemPart->type = TKEL_MEMPART_FIXED_BUFFER_SIZE;
	pMemPart->memPartSize = memPartSize;
	pMemPart->memPartAddress = memPartAddress;
	pMemPart->bufferSize = bufferSize;

	/* initialize the buffer pool */
	memset(memPartAddress, 0, (size_t)memPartSize);

	/* link all the free buffers */
	pMemPart->firstFreeBuffer = (TKEL_uint8*) memPartAddress;
	pBuffer = pMemPart->firstFreeBuffer;
	ppNextBuffer = (TKEL_uint8**) (pMemPart->firstFreeBuffer);
	count = memPartSize / bufferSize;
	for (i = 0; i < (count - 1); i++)
	{
		*ppNextBuffer = (TKEL_uint8*) (pBuffer + bufferSize);
		pBuffer = *ppNextBuffer;
		ppNextBuffer = (TKEL_uint8**) pBuffer;
	}
	*ppNextBuffer = NULL;
	pMemPart->lastFreeBuffer = pBuffer;

	/* return memory partition ID */
	*memPartID = (TKEL_memPart_id)pMemPart;

	return TKEL_NO_ERR;
}

/*=============================================================================
 *
 * TKEL_DeleteMemPart
 *
 * Parameters:
 *   memPartID       memory partition ID
 *
 * Description:
 *      Deletes a memory partition
 *
 * Returns:
 *   TKEL_NO_ERR   no error
 *   TKEL_BAD_ARG   wrong argument value
 *   TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_DeleteMemPart(
          TKEL_memPart_id   memPartID)
{
	TKEL_fixedSizeMemPart *pMemPart = (TKEL_fixedSizeMemPart*)memPartID;
	TKEL_sema_id semaphoreID;

	/* check parameters */
	if ((pMemPart == NULL) || ((pMemPart->type != TKEL_MEMPART_FIXED_BUFFER_SIZE) && (pMemPart->type != TKEL_MEMPART_VARIABLE_BUFFER_SIZE)))
	{
		/*
		* memPartID is NULL
		* or memPartID is not valid
		*/
		mTKEL_DBG_PRINT(("TKEL_DeleteMemPart: ERROR\n"));
		return TKEL_BAD_ARG;
	}

	/* lock */
	TKEL_WaitSemaphore(pMemPart->lock);

	semaphoreID = pMemPart->lock;

	/* deallocate memory partition descriptor */
	memset(pMemPart, 0, sizeof(TKEL_fixedSizeMemPart));
	TKEL_Free(pMemPart);

	/* unlock */
	TKEL_SignalSemaphore(semaphoreID);

	/* deallocate memory partition semaphore */
	TKEL_DeleteSemaphore(semaphoreID);

	return TKEL_NO_ERR;
}

/*=============================================================================
 *
 * TKEL_MemPartAlloc
 *
 * Parameters:
 *   memPartID       memory partition ID
 *   memorySize      Size of the memory buffer
 *   buffer         Points to the starting address of the memory buffer
 *
 * Description:
 *      Allocates a memory buffer
 *
 * Returns:
 *   TKEL_NO_ERR   no error
 *   TKEL_BAD_ARG   wrong argument value
 *   TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_MemPartAlloc(
          TKEL_memPart_id   memPartID,
          TKEL_msize      memorySize,
          void         **buffer)
{
	TKEL_fixedSizeMemPart   *pMemPart = (TKEL_fixedSizeMemPart*)memPartID;
	TKEL_uint8         **ppNextBuffer = NULL;
	TKEL_err          result = TKEL_NO_ERR;

	/* check parameters */
	if ((pMemPart == NULL) || ((pMemPart->type != TKEL_MEMPART_FIXED_BUFFER_SIZE) && (pMemPart->type != TKEL_MEMPART_VARIABLE_BUFFER_SIZE))
		|| (memorySize > pMemPart->bufferSize) || (buffer == NULL))
	{
		/*
		* memPartID is NULL
		* or memPartID is not valid
		* the requested size is different from the size of the buffers of this
		*   partition
		* or buffer is NULL
		*/
		mTKEL_DBG_PRINT(("TKEL_MemPartAlloc: ERROR\n"));
		return TKEL_BAD_ARG;
	}

	/* lock */
	TKEL_WaitSemaphore(pMemPart->lock);

	if (pMemPart->type == TKEL_MEMPART_FIXED_BUFFER_SIZE)
	{
		/* fixed size memory partition: allocate first free buffer */
		if (pMemPart->firstFreeBuffer != NULL)
		{
			/* a buffer is available */
			*buffer = (void*)(pMemPart->firstFreeBuffer);

			/* update the linked list */
			ppNextBuffer = (TKEL_uint8**)(pMemPart->firstFreeBuffer);
			pMemPart->firstFreeBuffer = *ppNextBuffer;
			*ppNextBuffer = NULL;
			if (pMemPart->firstFreeBuffer == NULL)
			{
			/* it was the last free buffer: the list is empty */
			pMemPart->lastFreeBuffer = NULL;
			}

			result = TKEL_NO_ERR;
		}
		else
		{
			/* no buffer is available */
			*buffer = NULL;
			mTKEL_DBG_PRINT(("TKEL_MemPartAlloc: ERROR\n"));
			result = TKEL_NOT_DONE;
		}
	}
	else
	{
		/* other types of memory partition are not yet supported */
		*buffer = NULL;
		mTKEL_DBG_PRINT(("TKEL_MemPartAlloc: ERROR\n"));
		result = TKEL_NOT_DONE;
	}

	/* unlock */
	TKEL_SignalSemaphore(pMemPart->lock);

	return(result);
}

/*=============================================================================
 *
 * TKEL_MemPartFree
 *
 * Parameters:
 *   memPartID       memory partition ID
 *   buffer         Starting address of the memory buffer
 *
 * Description:
 *      Releases a memory buffer
 *
 * Returns:
 *   TKEL_NO_ERR   no error
 *   TKEL_BAD_ARG   wrong argument value
 *   TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_MemPartFree(
          TKEL_memPart_id   memPartID,
          void         *buffer)
{
	TKEL_fixedSizeMemPart   *pMemPart = (TKEL_fixedSizeMemPart*)memPartID;
	TKEL_uint8         **ppNextBuffer = NULL;
	TKEL_err          result = TKEL_NO_ERR;

	/* check parameters */
	result = TKEL_MemPartCheckBuffer(memPartID, buffer);
	if (result != TKEL_NO_ERR)
	{
		return result;
	}

	/* lock */
	TKEL_WaitSemaphore(pMemPart->lock);

	if (pMemPart->type == TKEL_MEMPART_FIXED_BUFFER_SIZE)
	{
		/*
		* fixed size memory partition:
		* add buffer to the end of the linked list
		*/
		memset(buffer, 0, (size_t)(pMemPart->bufferSize));
		if (pMemPart->lastFreeBuffer != NULL)
		{
			/* the list is not empty */
			ppNextBuffer = (TKEL_uint8**)(pMemPart->lastFreeBuffer);
			*ppNextBuffer = (TKEL_uint8*)buffer;
			pMemPart->lastFreeBuffer = (TKEL_uint8*)buffer;
		}
		else
		{
			/* the list is empty */
			pMemPart->firstFreeBuffer = (TKEL_uint8*)buffer;
			pMemPart->lastFreeBuffer = (TKEL_uint8*)buffer;
		}
		ppNextBuffer = (TKEL_uint8**)(pMemPart->lastFreeBuffer);
		*ppNextBuffer = NULL;

		result = TKEL_NO_ERR;
	}
	else
	{
		/* other types of memory partition are not yet supported */
		mTKEL_DBG_PRINT(("TKEL_MemPartFree: ERROR\n"));
		result = TKEL_NOT_DONE;
	}

	/* unlock */
	TKEL_SignalSemaphore(pMemPart->lock);

	return(result);
	}

/*=============================================================================
 *
 * TKEL_MemPartCheckBuffer
 *
 * Parameters:
 *   memPartID      memory partition ID
 *   buffer       Starting address of the memory buffer to check
 *
 * Description:
 *      Check that an address is a memory buffer address
 *
 * Returns:
 *   TKEL_NO_ERR   no error
 *   TKEL_BAD_ARG   wrong address
 *
 *===========================================================================*/
TKEL_err TKEL_MemPartCheckBuffer(          /* Check a memory buffer. */
          TKEL_memPart_id   memPartID,
          void         *buffer)
{
	TKEL_fixedSizeMemPart   *pMemPart = (TKEL_fixedSizeMemPart*)memPartID;
	TKEL_err          result = TKEL_NO_ERR;

	/* check parameters */
	if ((buffer == NULL) ||(pMemPart == NULL) || ((pMemPart->type != TKEL_MEMPART_FIXED_BUFFER_SIZE) && (pMemPart->type != TKEL_MEMPART_VARIABLE_BUFFER_SIZE)))
	{
		/*
		* buffer is NULL
		* or memPartID is NULL
		* or memPartID is not valid
		*/
		mTKEL_DBG_PRINT(("TKEL_MemPartCheckBuffer: ERROR\n"));
		return TKEL_BAD_ARG;
	}

	/* lock */
	TKEL_WaitSemaphore(pMemPart->lock);

	if (pMemPart->type == TKEL_MEMPART_FIXED_BUFFER_SIZE)
	{
		/*
		* fixed size memory partition:
		* check that the address is in the partition and aligned.
		*/
		if ((buffer < pMemPart->memPartAddress) || (((TKEL_uint8*)pMemPart->memPartAddress + pMemPart->memPartSize) < (TKEL_uint8*)buffer)
			||	((((char*)buffer - (char*)pMemPart->memPartAddress) % pMemPart->bufferSize) != 0))
		{
			/*
			* the address is not in the partition
			* or not aligned
			*/
			mTKEL_DBG_PRINT(("TKEL_MemPartCheckBuffer: ERROR\n"));
			result = TKEL_BAD_ARG;
		}
		else
		{
			result = TKEL_NO_ERR;
		}
	}
	else
	{
		/* other types of memory partition are not yet supported */
		mTKEL_DBG_PRINT(("TKEL_MemPartCheckBuffer: ERROR\n"));
		result = TKEL_NOT_DONE;
	}

	/* unlock */
	TKEL_SignalSemaphore(pMemPart->lock);

	return(result);
}

#endif /* USE_TKEL_MEMORY_PARTITIONS */



#ifdef TKEL_DEBUG_STACK
/* This function can be used to verify the usage of task stack */
void TKEL_DebugPrintStackUsage(void)
{
    return;
}
#endif
/*=============================================================================
 *
 * TKEL_KillTask
 *
 * Parameters:
 *   taskID      task identifier
 *
 * Description:
 *      Kill a task
 *      This function ONLY KILL the task identified, NOT DELETE IT. Then a
 *      TKEL_DeleteTask is necessary.
 *
 *      Note: On OS21 a TKEL_DeleteTask without TKEL_KillTask may crash if
 *          the   main task function is not finite (in TKEL_DelayTask for 
 *          example if it is in a period) in the OS function
 *          task_wait(...,TIMEOUT_INFINITY);
 *
 * Returns:
 *   TKEL_NO_ERR   no error
 *   TKEL_BAD_ARG   wrong argument value
 *   TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_KillTask (TKEL_task_id taskID)
{

   return TKEL_NO_ERR;
}

/*=============================================================================
 *
 * TKEL_CreateTask_UnlimitStack
 * TKEL_CreateTask
 *
 * Parameters:
 *      priority   initial priority
 *   taskName      taskName
 *   stackSize   size of the stack (in bytes) limited to 32K in TKEL_CreateTask
 *   entry      address of the task entry function
 *   arg      address of an argument to passd to the task entry function
 *   taskID      pointer to a task identifier variable
 *
 * Description:
 *      Creates and starts a task.
 *
 * Returns:
 *      TKEL_NO_ERR   no error
 *   TKEL_BAD_ARG   wrong argument value
 *   TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_CreateTask_UnlimitStack (TKEL_prio priority,
          char *taskName,
                            TKEL_msize stackSize,
          void (*entry) (void*),
          void *arg,
          TKEL_task_id *taskID)
{

    stackSize = ((stackSize<0x8000)?stackSize:0x7ffc);

    return TKEL_CreateTask(priority,
                taskName,
                stackSize,
                entry,
                arg,
                taskID);

}
