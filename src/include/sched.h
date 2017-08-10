#ifndef __INCLUDE_SCHEDULING_H__
#define __INCLUDE_SCHEDULING_H__
#include <arch.h>
#include <task/task.h>
void clock_callback(pt_regs_t *regs);
void schedule(void);
void wakeup_task(struct task_struct *task);
#endif
