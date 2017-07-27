#ifndef __INCLUDE_CHAR_DEV_H__
#define __INCLUDE_CHAR_DEV_H__
#include <types.h>
typedef struct char_dev
{
    const char *name;
    bool is_readable;
    bool is_writeable;
    struct char_ops
    {
        int (*init)(void);
        bool (*device_vaild)(void);
        const char *(*get_desc)(void);
        int (*read)(void *,uint32_t);
        int (*write)(const void *,uint32_t);
        int (*ioctl)(int,int);
    } ops;
    struct char_dev *next;
} char_dev_t;
extern char_dev_t *char_devs;
void char_dev_init(void);
int add_char_dev(char_dev_t *cdev);
extern char_dev_t keyboard_dev;
#endif
