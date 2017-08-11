#include <debug.h>
#include <common.h>
#include <arch.h>
#include <sched.h>
#include "clock.h"
#define FREQUENCY 100
#define IO_TIMER 0x040
#define TIMER_FREQ 1193180
#define TIMER_MODE (IO_TIMER + 3)
#define TIMER_SEL0 0x00
#define TIMER_RATEGEN 0x04
#define TIMER_CLOCK 0x06
#define TIMER_16BIT 0x30
void clock_init(void)
{
    register_interrupt_handler(IRQ0,clock_callback);
    uint32_t divisor = TIMER_FREQ / FREQUENCY;
    outb(TIMER_MODE,TIMER_SEL0 | TIMER_RATEGEN | TIMER_16BIT);
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
    outb(IO_TIMER,low);
    outb(IO_TIMER,high);
}
