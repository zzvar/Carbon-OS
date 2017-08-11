#ifndef __INCLUDE_PIC_H__
#define __INCLUDE_PIC_H__
#include <arch.h>
void init_interrupt_chip(void);
void clear_interrupt_chip(uint32_t intr_no);
#endif
