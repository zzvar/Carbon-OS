#ifndef _FS_SFS_SFS_H_
#define _FS_SFS_SFS_H_
#include <types.h>
struct sfs_super_block {
    uint16_t s_type;
    uint32_t s_inode_count;
    uint32_t s_block_count;
    uint32_t s_block_size;
    uint32_t s_max_file;
    uint32_t s_root_inode;
    uint32_t in_secs;
    uint32_t bl_secs;
    uint32_t im_start;
    uint32_t im_secs;
    uint32_t bm_start;
    uint32_t bm_secs;
    uint32_t in_start;
    uint32_t bl_start'
};
#define P_BLOCK_MAP_SIZE 32
#define PP_BLOCK_MAP_SIZE 32
struct sfs_inode {
    uint32_t i_type;
    uint32_t i_ino;
    time_t i_atime;
    time_t i_mtime;
    time_t i_ctime;
    uint32_t i_size;
    uint32_t i_blocks;
    uint32_t i_bytes;
    uint32_t p_block_map[P_BLOCK_MAP_SIZE];
    uint32_t pp_block_map[PP_BLOCK_MAP_SIZE];
};
#define SFS_MAX_FILE_NAME 123
struct sfs_direct {
    char d_name[SFS_MAX_FILE_NAME + 1];
    uint32_t inode;
};
#endif
