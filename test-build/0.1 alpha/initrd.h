#ifndef _INITRD_H_
#define _INITRD_H_
#include "common.h"
#include "fs.h"
typedef struct
{
    u32int nfiles;
} initrd_header_t;
typedef struct
{
    u8int magic;
    s8int name[64];
    u32int offset;
    u32int length;
} initrd_file_header_t;
fs_node_t *initialise_initrd(u32int location);
#endif