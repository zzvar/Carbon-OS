#ifndef __INCLUDE_BLOCK_DEV_H__
#define __INCLUDE_BLOCK_DEV_H__
#include <types.h>
typedef enum io_type
{
    IO_READ = 0,
    IO_WRITE = 1
} io_type_t;
typedef struct io_request
{
    io_type_t io_type;
    uint32_t secno;
    uint32_t nsecs;
    void *buffer;
    uint32_t bsize;
} io_request_t;
typedef struct block_dev
{
    const char *name;
    uint32_t block_size;
    struct block_ops
    {
        int (*init)(void);
        bool (*device_vaild)(void);
        const char *(*get_desc)(void);
        int (*get_nr_block)(void);
        int (*request)(io_request_t *);
        int (*ioctl)(int,int);
    } ops;
    struct block_dev *next;
} block_dev_t;
extern block_dev_t *block_devs;
void block_dev_init(void);
int add_block_dev(block_dev_t *bdev);
extern block_dev_t ide_main_dev;
#endif
