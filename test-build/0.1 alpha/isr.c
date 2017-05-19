#include "common.h"
#include "isr.h"
#include "monitor.h"
isr_t interrupt_handlers[256];
void register_interrupt_handler(u8int n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}
void isr_handler(registers_t regs)
{
    u8int int_no = regs.int_no & 0xFF;
    if (interrupt_handlers[int_no] != 0)
    {
        isr_t handler = interrupt_handlers[int_no];
        handler(&regs);
    }
    else
    {
        monitor_write("unhandled interrupt: ");
        monitor_write_hex(int_no);
        monitor_put('\n');
        for(;;);
    }
}
void irq_handler(registers_t regs)
{
    if (regs.int_no >= 40)
    {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(&regs);
    }
}