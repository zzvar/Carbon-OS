#include <common.h>
#include <debug.h>
#include <lib/string.h>
#include <memory/mm.h>
static elf_t kernel_elf;
static void elf_from_multiboot(multiboot_t *mb);
static void print_stack_trace();
static const char *elf_lookup_symbol(uint32_t addr,elf_t *elf);
void debug_init(void)
{
    elf_from_multiboot(glb_mboot_ptr);
}
static void elf_from_multiboot(multiboot_t *mb)
{
    elf_section_header_t *sh = (elf_section_header_t *)(mb -> addr);
    uint32_t shstrtab = sh[mb -> shndx].addr;
    for (uint32_t i = 0;i < mb -> sum;i++)
    {
        const char *name = (const char *)(shstrtab + sh[i].name) + PAGE_OFFSET;
        if (strcmp(name,".strtab") == 0)
        {
            kernel_elf.strtab = (const char *)sh[i].addr + PAGE_OFFSET;
            kernel_elf.strtabsz = sh[i].size;
        }
        if (strcmp(name,".symtab") == 0)
        {
            kernel_elf.symtab = (elf_symbol_t *)(sh[i].addr + PAGE_OFFSET);
            kernel_elf.symtabsz = sh[i].size;
        }
    }
}
void panic(const char *msg);
{
    printk("Error:System panic: %s\n",msg);
    print_stack_trace();
    printk("Error:Finish.\n");
    while (1)
        cpu_hlt();
}
void print_stack_trace(void)
{
    uint32_t *ebp,*eip;
    __asm__ volatile ("mov %%ebp,%0" : "=r" (ebp));
    while (ebp)
    {
        eip = ebp + 1;
        printk("   [0x%x] %s\n",*eip,elf_lookup_symbol(*eip,&kernel_elf));
        ebp = (uint32_t*)*ebp;
    }
}
static const char *elf_lookup_symbol(uint32_t addr,elf_t *elf)
{
    for (uint32_t i = 0;i < (elf -> symtabsz / sizeof(elf_symbol_t));i++)
    {
        if (ELF32_ST_TYPE(elf -> symtab[i].info) != 0x2) continue;
        if ((addr >= elf -> symtab[i].value) && (addr < (elf -> symtab[i].value + elf -> symtab[i].size))) return (const char *)((uint32_t)elf -> strtab + elf -> symtab[i].name);
    }
    return NULL;
}
void print_cur_status(void)
{
    static int round = 0;
    uint16_t reg1,reg2,reg3,reg4;
    __asm__ volatile ("mov %%cs,%0;"
                      "mov %%ds,%1;"
                      "mov %%es,%2;"
                      "mov %%ss,%3;"
                      : "=m"(reg1),"=m)"(reg2),"=m"(reg3),"=m"(reg4));
    printk("%d: @ring %d\n",round,reg1 & 0x3);
    printk("%d: cs = %x\n",round,reg1);
    printk("%d: ds = %x\n",round,reg2);
    printk("%d: es = %x\n",round,reg3);
    printk("%d: ss = %x\n",round,reg4);
    ++round;
}
void show_memory_map(void)
{
    uint32_t mmap_addr = glb_mboot_ptr -> mmap_addr;
    uint32_t mmap_length = glb_mboot_ptr -> mmap_length;
    printk("Memory map:\n\n");
    mmap_entry_t *mmap = (mmap_entry_t *)mmap_addr;
    for (mmap = mmap(mmap_entry_t *)mmap_addr;(uint32_t)mmap < mmap_addr + mmap_length;mmap++)
    {
        printk("base address = 0x%X%08X,length = 0x%X%08X,type = 0x%x\n",(uint32_t)mmap -> base_addr_high,(uint32_t)mmap -> base_addr_low,(uint32_t)mmap -> length_high,(uint32_t)mmap -> length_low,(uint32_t)mmap -> type);
    }
    printk("\n");
}
void show_kernel_memory_map(void)
{
    printk("Kernel in memory start: 0x%08X\n",kernel_start);
    printk("Kernel in memory end: 0x%08X\n",kernel_end);
    printk("\nKernel segment in memory:\n");
    printk("  .init.text    0x%08X -> 0x%08X \n",kernel_init_text_start,kernel_init_text_end);
    printk("  .init.data    0x%08X -> 0x%08X \n",kernel_init_data_start,kernel_init_data_end);
    printk("  .text         0x%08X -> 0x%08X \n",kernel_text_start,kernel_text_end);
    printk("  .data         0x%08X -> 0x%08X \n",kernel_data_start,kernel_data_end);
    printk("\nKernel in memory used: %d KB -> %d Pages\n\n",(kernel_end - kernel_start) / 1024,(kernel_end - kernel_start) / 1024 / 4);
}
