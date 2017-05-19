#include "syscall.h"
#include "isr.h"
#include "monitor.h"
static void syscall_handler(registers_t *regs);
DEFN_SYSCALL1(monitor_write, 0, const char*);
DEFN_SYSCALL1(monitor_write_hex, 1, const char*);
DEFN_SYSCALL1(monitor_write_dec, 2, const char*);
static void *syscalls[3] =
{
    &monitor_write,
    &monitor_write_hex,
    &monitor_write_dec,
};
u32int num_syscalls = 3;
void initialise_syscalls()
{
    register_interrupt_handler (0x80, &syscall_handler);
}
void syscall_handler(registers_t *regs)
{
    if (regs->eax >= num_syscalls)
        return;
    void *location = syscalls[regs->eax];
    int ret;
    asm volatile (" \
      push %1; \
      push %2; \
      push %3; \
      push %4; \
      push %5; \
      call *%6; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
    " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
    regs->eax = ret;
}