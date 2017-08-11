#ifndef __ARCH_H__
#define __ARCH_H__
#include <memory/gdt.h>
#include <interrupt/intr.h>
#include <driver/pic.h>
#include <driver/clock.h>
#include <driver/console.h>
#include <task/task.h>
void arch_init(void);
#endif
