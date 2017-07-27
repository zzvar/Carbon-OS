#ifndef __INCLUDE_DEBUG_H__
#define __INCLUDE_DEBUG_H__
#include <arch.h>
#include <vargs.h>
#include <elf.h>
#include <errno.h>
extern uint8_t kernel_init_text_start[];
extern uint8_t kernel_init_text_end[];
extern uint8_t kernel_init_data_start[];
extern uint8_t kernel_init_data_end[];
extern uint8_t kernel_text_start[];
extern uint8_t kernel_text_end[];
extern uint8_t kernel_data_start[];
extern uint8_t kernel_data_end[];
#define assert(x,info) do { \
	if (!(x)) panic(info); \
	} while (0)
void debug_init(void);
void show_memory_map(void);
void show_kernel_memory_map(void);
void panic(const char *msg);
void print_cur_status(void);
void printk(const char *format,...);
void cprintk(real_color_t back,real_color_t fore,const char *format,...);
#endif
