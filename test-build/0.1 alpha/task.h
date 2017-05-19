#ifndef _TASK_H_
#define _TASK_H_
#include "common.h"
#include "paging.h"
#define KERNEL_STACK_SIZE 2048
typedef struct task
{
    int id;
    u32int esp, ebp;
    u32int eip;
    page_directory_t *page_directory;
    u32int kernel_stack;
    struct task *next;
} task_t;
void initialise_tasking();
void task_switch();
int fork();
void move_stack(void *new_stack_start, u32int size);
int getpid();
#endif