#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"
#include "task.h"
#include "syscall.h"
extern u32int placement_adsress;
u32int inital_esp;
int main(struct multiboot *mboot_ptr,u32int inital_stack)
{
	inital_esp = inital_stack;
	init_descriptor_tables();
	monitor_clear();
	asm volatile("sti");
	init_timer(50);
	ASSERT(mboot_ptr->mods_count > 0);
	u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
	u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
	placement_adsress = initrd_end;
	initialise_paging();
	initialise_tasking();
	fs_root = initialise_initrd(initrd_location);
	initialise_syscalls();
	switch_user_mode();
	syscall_monitor_write("Hello Carbon OS User mode world!\n");
	return 0;
}