#include <init.h>
#include <arch.h>
#include <common.h>
#include <debug.h>
#include <sched.h>
#include <memory/memory.h>
#include <fs.h>
#include <kio.h>
static void kthread_test(void) __UNUSED__;
void kernal_init(void)
{
    debug_init();
    arch_init();
    memory_init();
    task_init();
    fs_init();
    enable_intr();
    uint8_t ch = 0;
    while (true)
    {
        if ((ch = getchar()) != 0)
        {
            if (ch == 80)
                console_view_down(1);
            else if (ch == 72)
                console_view_up(1);
        }
    }
    while (true)
        cpu_init();
}
static int init_main(void *args)
{
    cprintk(rc_black,rc_red,"Now,It's %s thread pid = %d args: %s\n\n",current -> name,current -> pid,(const char *)args);
    while (true)
    {
        cprintk(rc_black,rc_blue,"C");
        uint32_t i = 100000;
        while (i--);
    }
    return 0;
}
static int user_mode_test_main(void *args)
{
    cprintk(rc_black,rc_blue,"It's %s thread pid = %d args: %s\n",current -> name,current -> pid,(const char *)args);
    cprintk(rc_black,rc_light_brown,"\nTest syscall now:\n");
    asm volatile ("mov $0,$eax");
    asm volatile ("int $0x80");
    printk("\n");
    while (true)
    {
        cprintk(rc_black,rc_green,"A");
        uint32_t i = 100000;
        while (i--);
    }
    return 0;
}
static void kthread_test(void)
{
    pid_t pid;
    pid = kernel_thread(user_mode_test_main,"ring0 -> ring3",0);
    assert(pid > 0,"user_mode_test thread error!");
    glb_init_task = find_task(pid);
    set_proc_name(glb_init_task,"user_mode_test");
    cprintk(rc_black,rc_green,"\nIt's %s thread pid = %d\n\n",current -> name,current -> pid);
    pid = kernel_thread(init_main,"Here a new thread borned",0);
    assert(pid > 0,"init_task error!");
    glb_init_task = find_task(pid);
    set_proc_name(glb_init_task,"init");
    while (true)
    {
        cprintk(rc_black,rc_red,"B");
        uint32_t i = 100000;
        while (i--);
    }
}
