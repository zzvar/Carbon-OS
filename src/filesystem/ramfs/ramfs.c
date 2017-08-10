#include <fs.h>
static struct super_block *ramfs_read_super(struct super_block *sb);
struct filesystem fs_ramfs = {
    .name = "ramfs",
    .type = RAMFS_T,
    .read_super = ramfs_read_super
};
static struct inode *ramfs_alloc_inode(struct super_block *sb);
static void ramfs_destroy_inode(struct inode *inode);
static void ramfs_write_super(struct super_block *sb);
static int ramfs_sync_fs(struct super_block *sb);
static struct super_block_ops ramfs_sb_ops = {
    .alloc_inode = ramfs_alloc_inode,
    .destroy_inode = ramfs_destroy_inode,
    .write_super = ramfs_write_super,
    .sync_fs = ramfs_sync_fs
};
static struct super_block *ramfs_read_super(struct super_block *sb)
{
    sb -> s_type = RAMFS_T;
    sb -> s_ops = &ramfs_sb_ops;
    return sb;
}
static struct inode *ramfs_alloc_inode(__UNUSED__ struct super_block *sb)
{
    return alloc_inode();
}
static void ramfs_destroy_inode(struct inode *inode)
{
    free_inode(inode);
}
static void ramfs_write_super(__UNUSED__ struct super_block *sb)
{
    
}
static int ramfs_sync_fs(__UNUSED__ struct super_block *sb)
{
    return 0;
}
