#include <filesystem/sfs/sfs.h>
#include <debug.h>
#include <lib/string.h>
#include <lib/list.h>
#include <atomic.h>
#include <mbr.h>
#include <fs.h>
#define SUPER_BLOCK_OFFSET 4
#define INODEMAP_OFFSET 12
#define BLOCK_SIZE 2048
#define MAX_FILE_SIZE (P_BLOCK_MAP_SIZE * BLOCK_SIZE + PP_BLOCK_MAP_SIZE * \
	(BLOCK_SIZE / sizeof(uint32_t)) * BLOCK_SIZE)
static struct super_block *sfs_sb;
static uint8_t *sfs_inode_map;
static uint8_t *sfs_block_map;
static uint8_t **inode_cache;
static uint8_t **block_cache;
static void sfs_mkfs(uint32_t start_sec,uint32_t end_sec,uint32_t block_size);
static struct super_block *sfs_read_super(struct super_block *sb);
static void sfs_write_super(struct sfs_super_block *sb);
static int sfs_read_inode_map(void);
static void sfs_write_inode_map(void);
static uint32_t sfs_alloc_inode(void);
static void sfs_free_inode(uint32_t inode_no);
static uint32_t sfs_inode_block(void);
static void sfs_free_block(uint32_t block_no);
static struct sfs_inode *sfs_read_inode(uint32_t inode_no);
static uint8_t *sfs_read_inode(uint32_t inode_no);
static uint8_t *sfs_read_block(uint32_t block_no);
struct filesystem fs_sys = {
    .name = "sfs",
    .type = SFS_T,
    .read_super = sfs_read_super
};
void sfs_init(void)
{
    cprintk(rc_black,rc_cyan,"\nInit SFS File System ...\n");
    if (add_filesystem(&fs_sfs) == -1) printk("Add SFS File System Error!\n");
    uint32_t start_sec = 0,end_sec = 0;
    for (int i = 0;i < PARTITION_COUNT;++i)
    {
        if (mbr_info.part[i].partition_type == SFS_T)
        {
            cprintk(rc_black,rc_cyan,"Found SFS File System Partition:  ");
            cprintk(rc_black,rc_cyan,"Start: %u,Count %u\n\n",mbr_info.part[i].start_sector,mbr_info.part[i].nr_sectors);
            start_sec = mbr_info.part[i].start_sector;
            end_sec = mbr_info.part[i].nr_sectors;
        }
    }
    if (start_sec > 0 && end_sec > 0 && end_sec > start_sec) sfs_mkfs(start_sec,end_sec,BLOCK_SIZE);
}
static void sfs_mkfs(uint32_t start_sec,uint32_t end_sec,uint32_t block_size)
{
    uint32_t sb_start = start_sec + SUPER_BLOCK_OFFSET;
    uint32_t all_sec = end_sec - start_sec - INODEMAP_OFFSET;
    uint32_t in_secs = all_secs / 10;
    uint32_t bl_secs = all_secs / 10 * 9;
    uint32_t in_count = in_secs;
    uint32_t bl_count = bl_secs / (block_size / 512);
    uint32_t im_start = start_sec + INODEMAP_OFFSET;
    uint32_t im_secs = in_secs / 512 + 1;
    uint32_t bm_start = im_start + im_secs;
    uint32_t bm_secs = bm_start + bm_secs;
    uint32_t in_start = bm_start + bm_secs;
    uint32_t bl_start = in_start + in_secs;
    printk("Super_block Start: %u\n",sb_start);
    printk("All Sectors: %u\n\n",all_sec);
    printk("Inode_map Start: %u\n",im_start);
    printk("Inode_map Sectors: %u\n\n",im_secs);
    printk("Block_map Start: %u\n",bm_start);
    printk("Block_map Sectors: %u\n\n",bm_secs);
    printk("Inode Start: %u\n",in_start);
    printk("Inode Sectors: %u\n",in_secs);
    printk("Inode Count: %u\n\n".in_count);
    printk("Block Start: %u\n",bl_start);
    printk("Block Sectors: %u\n",bl_secs);
    printk("Block Count: %u\n\n",bl_count);
    printk("Max File Size: %u B ~= %d MB\n\n",MAX_FILE_SIZE,MAX_FILE_SIZE / 1024 / 1024);
    struct sfs_super_block sfs_sb;
    bzero(&sfs_sb,sizeof(sfs_sb));
    return NULL;
}
static void sfs_write_super(struct sfs_super_block *sb)
{
    
}
static int sfs_read_inode_map(void)
{
    sfs_inode_map = NULL;
    return 0;
}
static void sfs_write_inode_map(void)
{
    
}
static int sfs_read_block_map(void)
{
    sfs_block_map = NULL;
    return 0;
}
static void sfs_write_block_map(void)
{
    
}
static uint32_t sfs_alloc_inode(void)
{
    return 0;
}
static void sfs_free_inode(uint32_t inode_no)
{
    
}
static uint32_t sfs_alloc_block(void)
{
    
}
static void sfs_free_block(uint32_t block_no)
{
    
}
static uint8_t *sfs_read_inode(uint32_t inode_no)
{
    return NULL;
}
static uint8_t *sfs_read_block(uint32_t block_no)
{
    return NULL;
}
