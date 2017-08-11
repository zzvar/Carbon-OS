#include <common.h>
#include "pic.h"
#define IO_PIC1 (0x20)
#define IO_PIC2 (0xA0)
#define IO_PIC1C (IO_PIC1 + 1)
#define IO_PIC2C (IO_PIC2 + 1)
void init_interrupt_chip(void)
{
    outb(IO_PIC1,0x11);
    outb(IO_PIC2,0x11);
    outb(IO_PIC1C,0x20);
    outb(IO_PIC2C,0x28);
    outb(IO_PIC1C,0x04);
    outb(IO_PIC2C,0x02);
    outb(IO_PIC1C,0x01);
    outb(IO_PIC2C,0x01);
    outb(IO_PIC1C,0x0);
    outb(IO_PIC2C,0x0);
}
void clear_interrupt_chip(uint32_t intr_no)
{
    if (intr_no >= 40) outb(IO_PIC2,0x20);
    outb(IO_PIC1,0x20);
}
