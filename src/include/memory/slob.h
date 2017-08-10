#ifndef __INCLUDE_MEMORY_SLOB_H__
#define __INCLUDE_MEMORY_SLOB_H__
void slob_init(void);
void *kmalloc(uint32_t size);
void kfree(void *addr);
#endif
