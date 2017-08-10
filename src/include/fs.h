#ifndef __INCLUDE_FS_H__
#define __INCLUDE_FS_H__
#include <types.h>
#include <spinlock.h>
#include <block_dev.h>
#define RAMFS_T 0xA0
#define SFS_T 0xAA
struct super_block;
struct super_ops;
struct inode;
struct inode_ops;
struct dentry;
struct dentry_ops;
struct file;
struct file_ops;
struct filesystem
{
    const char *name;
    uint8_t type;
    struct super_block *(*read_super)(struct super_block *);
    struct list_head fs_supers;
    struct filesystem *next;
}
struct super_block
{
    uint16_t s_type;
    uint16_t s_inode_count;
    uint16_t s_block_count;
    uint16_t s_block_size;
    uint16_t s_max_size;
    uint16_t s_root_inode;
    void *impl_sb;
    struct list_head s_list;
    block_dev_t *bdev;
    struct dentry *s_root
    struct super_block_ops *s_ops;
};
struct super_block_ops
{
    struct inode *(*alloc_inode)(struct super_block *);
    void (*destroy_inode)(struct inode *);
    void (*write_super)(struct super_block *);
    int (*sync_fs)(struct super_block *);
};
#define S_DIR 0x1
#define S_FILE 0x2
struct inode
{
    uint32_t i_type;
    uint32_t i_ino;
    time_t i_atime;
    time_t i_mtime;
    time_t i_ctime;
    uint32_t i_size;
    uint32_t i_blocks;
    uint32_t i_bytes;
    void *impl_in;
    spinlock_t i_lock;
    atomic_t i_count;
    struct super_block *i_sb;
    struct list_head i_list;
};
struct inode_ops
{
    int (*create)(struct inode *,struct dentry *);
    int (*rm)(struct inode *,struct dentry *);
    int (*mkdir)(struct inode *,struct dentry *);
    int (*rmdir)(struct inode *,struct dentry *);
    int (*rename)(struct inode *,struct dentry *,struct inode,struct *dentry);
};
#define MAX_FILE_NAME 123
struct dirent
{
    char d_name[MAX_FILE_NAME + 1];
    uint32_t inode;
};
struct dentry
{
    atomic_t d_count;
    spinlock_t d_lock;
    char d_name[MAX_FILE_NAME + 1];
    uint32_t d_status;
    struct dentry *d_parent;
    struct list_head d_subdirs;
    struct list_head d_child;
    bool is_mounted;
    struct super_block *d_sb;
    struct inode *d_inode;
    struct dentry_ops *d_ops;
};
struct dentry_ops
{
    int (*del)(struct dentry *);
};
#define MAX_PATH 1024
struct file
{
    atomic_t f_count;
    spinlock_t f_lock;
    char f_path[MAX_PATH + 1];
    struct dentry *f_dentry;
    uint32_t f_openflag;
    uint32_t f_pos;
    struct file_ops *f_ops;
};
strcut file_ops
{
    int (*read)(struct file *,char *,uint32_t);
    int (*write)(struct file *,const char *,uint32_t);
    int (*open)(struct inode *,struct file *);
    int (*fsync)(struct file *);
    int (*close)(struct inode *,struct file *);
};
#define MAX_OPEN_FILE 64
struct file_struct
{
    spinlock_t fs_lock;
    struct vfsmount *vfsmount;
    struct file *file_array[MAX_OPEN_FILE];
};
struct vfsmount
{
    const char *mnt_devname;
    struct super_block *mnt_sb;
    struct dentry *mnt_root;
    struct dentry *mnt_mount_point;
    struct vfsmount *mnt_parent;
};
extern struct filesystem *file_systems;
extern struct filesystem *filesystem_ramfs;
void filesystem_init(void);
int add_filesystem(struct filesystem *fs)
struct super_block *alloc_super_block(void);
struct super_block_ops *alloc_super_block_ops(void);
struct inode *alloc_inode(void);
struct inode_ops *alloc_inode_ops(void);
struct dentry *alloc_dentry(void);
struct dentry_ops *alloc_dentry_ops(void);
struct file *alloc_file(void);
struct file_ops *alloc_file_ops(void);
void free_super_block(struct super_block *sb);
void free_super_block_ops(struct super_ops *sb_ops);
void free_inode(struct inode *inode);
void free_inode_ops(struct inode_ops *inode_ops);
void free_dentry(struct dentry *dentry);
void free_dentry_ops(struct dentry_ops *dentry_ops);
void free_file(struct file *file);
void free_file_ops(struct file *file_ops);
int vfs_open(const char *filename,uint32_t openflag);
int vfs_close(int fd);
int vfs_read(int fd,char *buff,size_t size);
int vfs_write(int fd,const char *buff,size_t size);
#endif
