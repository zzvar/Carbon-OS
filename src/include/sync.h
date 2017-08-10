#ifndef __INCLUDE_SYNC_H__
#define __INCLUDE_SYNC_H__
#include <common.h>
#include <types.h>
#include <arch.h>
static inline bool __intr_store(void)
{
    if (read_eflags() & FL_IF)
    {
        disable_intr();
        return true;
    }
    return false;
}
static inline void __intr_restore(bool flag)
{
    if (flag) enable_intr();
}
#define local_intr_store(x) do { x = __intr_store(); } while (0)
#define local_intr_restore(x) __intr_restore(x);
#endif
