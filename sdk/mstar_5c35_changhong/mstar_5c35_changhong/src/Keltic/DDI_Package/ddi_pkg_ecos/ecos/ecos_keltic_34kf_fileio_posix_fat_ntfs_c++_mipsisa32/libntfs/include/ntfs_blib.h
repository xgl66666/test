#ifndef _NTFS_BLIB_H_
#define _NTFS_BLIB_H_

#include <linux/rbtree.h>
#include <linux/list.h>


typedef struct {
    cyg_uint32  n_gets;     // number of block gets
    cyg_uint32  n_reads;    // number of block reads
    cyg_uint32  n_writes;   // number of block writes
} cyg_blib_stat_t;



typedef struct {
    struct list_head	list_head;       // head of block list
    struct rb_root		rb_root;         // block red-black tree root
    void*		device;          // Pointer to device object
    cyg_uint32			block_size;      // block size
    cyg_uint32			block_size_log2; // block size log2
    cyg_uint8*			mem_base;        // memory base
    cyg_uint32			mem_size;        // memory size
    struct list_head	free_list_head;  // list of free blocks

    void*				dblock_phy_array; //array of block in phyiscal address order
    cyg_uint32			dblock_phy_array_size;
    cyg_uint32			dblock_reserved_read_cache_size; // array reserved for read cache, to enhance FAT table browsing performance
    cyg_uint32			last_phy_dblock_w_search_pos;
    cyg_uint32			last_phy_dblock_r_search_pos;
} cyg_blib_t;

// --------------------------------------------------------------------
// Creates a block lib instance
//
//   mem_base   - block cache memory base
//   mem_size   - block cache memory size
//   block_size - block size
//   bl         - block lib instance space holder
//
//   returns ENOERR if create succeded
//

int cyg_blib_create_NTFS(void*			dev,
                    void               *mem_base,
                    cyg_uint32          mem_size,
                    cyg_uint32          block_size,
                    cyg_blib_t         *bl);

// --------------------------------------------------------------------
// Creates a block lib instance on top of IO system
//   (cyg_io_bread and cyg_io_bwrite)
//
//   handle     - cyg_io_handle_t
//   mem_base   - block cache memory base
//   mem_size   - block cache memory size
//   block_size - block size
//   bl         - block lib instance space holder
//
//   returns ENOERR if create succeded
//

int cyg_blib_io_create_NTFS(void*			dev,
                       void               *mem_base,
                       cyg_uint32          mem_size,
                       cyg_uint32          block_size,
                       cyg_blib_t         *bl);

// --------------------------------------------------------------------
// Deletes a block lib instance
//
//   bl - block lib instance
//
//   The block cache is synced before
//
//   returns ENOERR if delete succeded
//

int cyg_blib_delete_NTFS(cyg_blib_t *bl);

// --------------------------------------------------------------------
// Reads a number of blocks
//
//   bl  - block lib instance
//   buf - block buffer ptr
//   len - number of blocks to read
//   pos - starting block number
//
//   returns ENOERR if read succeded
//

int cyg_blib_bread_NTFS(cyg_blib_t *bl,
                   void       *buf,
                   cyg_uint32 *len,
                   cyg_uint32  pos);

// --------------------------------------------------------------------
// Writes a number of blocks
//
//   bl  - block lib instance
//   buf - block buffer ptr
//   len - number of blocks to write
//   pos - starting block number
//
//   returns ENOERR if write succeded
//

int cyg_blib_bwrite_NTFS(cyg_blib_t *bl,
                    const void *buf,
                    cyg_uint32 *len,
                    cyg_uint32  pos);

// --------------------------------------------------------------------
// Reads data
//
//   bl   - block lib instance
//   buf  - data buffer ptr
//   len  - number of bytes to read
//   bnum - starting block number
//   pos  - starting position inside starting block
//
//   returns ENOERR if read succeded
//
//   The block number is automatically adjusted if
//   position is greater than block size
//

int cyg_blib_read_NTFS(cyg_blib_t *bl,
                  void       *buf,
                  cyg_uint64 *len,
                  cyg_uint32  bnum,
                  cyg_uint64  pos);

// --------------------------------------------------------------------
// Writes data
//
//   bl   - block lib instance
//   buf  - data buffer ptr
//   len  - number of bytes to write
//   bnum - starting block number
//   pos  - starting position inside starting block
//
//   returns ENOERR if write succeded
//
//   The block number is automatically adjusted if
//   position is greater than block size
//

int cyg_blib_write_NTFS(cyg_blib_t *bl,
                   const void *buf,
                   cyg_uint64 *len,
                   cyg_uint32  bnum,
                   cyg_uint64  pos);

// --------------------------------------------------------------------
// Syncs block cache - ie write modified blocks
//
//   bl - block lib instance
//
//   returns ENOERR if sync succeded
//

int cyg_blib_sync_NTFS(cyg_blib_t *bl);

// --------------------------------------------------------------------
// Syncs block - ie write if modified
//
//   bl  - block lib instance
//   num - block number to sync
//
//   returns ENOERR if sync succeded
//

int cyg_blib_sync_block_NTFS(cyg_blib_t *bl, cyg_uint32 num);

// --------------------------------------------------------------------
// Flushes block cache
//
//   bl  - block lib instance
//
//   returns ENOERR if flush succeded
//
//   The block cache is synced before
//

int cyg_blib_flush_NTFS(cyg_blib_t *bl);

// --------------------------------------------------------------------
// Sets block size
//
//   bl         - block lib instance
//   block_size - new block size
//
//   returns ENOERR if set succeded
//
//   The block cache is synced before
//

int cyg_blib_set_block_size_NTFS(cyg_blib_t *bl, cyg_uint32 block_size);

// --------------------------------------------------------------------
// Gets block size
//
//   bl  - block lib instance
//
//   returns the current block size

static inline cyg_uint32 cyg_blib_get_block_size_NTFS(cyg_blib_t *bl)
{
    return bl->block_size;
}

// --------------------------------------------------------------------
// Gets log2 of block size
//
//   bl  - block lib instance
//
//   returns log2 of the current block size

static inline cyg_uint32 cyg_blib_get_block_size_log2_NTFS(cyg_blib_t *bl)
{
    return bl->block_size_log2;
}

// --------------------------------------------------------------------
// Gets block cache statistics
//
//   bl - block lib instance
//
//   returns ENOERR if get succeded
//

int cyg_blib_get_stat_NTFS(cyg_blib_t *bl, cyg_blib_stat_t *stat);

// --------------------------------------------------------------------------


#endif   //  _NTFS_BLIB_H_


