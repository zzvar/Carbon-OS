#include <debug.h>
#include <sync.h>
#include <lib/list.h>
#include "sched.h"
void clock_callback(__UNUSED__ pt_regs_t *regs)
{
    schedule();
}
void schedule(void)
{
    struct task_struct *task_next = NULL;
    struct list_head *le = NULL,*last = NULL;
    bool intr_flag = false;
    local_intr_store(intr_flag)
    {
        current -> need_resched = false;
        le = last = (current == glb_idle_task) ? &task_list : &(current -> list);
        for (le = le -> next;le != last;le = le -> next)
        {
            if (le != &task_list) 
            {
                task_next = le_to_task(le);
                if (task_next -> state == TASK_RUNNABLE)
                    break;
            }
        }
        if ((!task_next) || (task_next -> state != TASK_RUNNABLE))
            task_init = glb_idle_task;
        task_next -> runs_time++;
        if (task_next != current)
        {
            task_run(task_next);
        }
    }
    local_intr_restore(intr_flag);
}
void wakeup_task(struct task_struct *task)
{
    if (task -> state != TASK_ZOMBIE)
        task -> state = TASK_RUNNABLE;
}
