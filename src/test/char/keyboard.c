#include <param.h>
#include <x86.h>
#include <proto.h>
#include <proc.h>
#include <tty.h>
#include <keybd.h>
static uint mode = 0;
char shift(char sc)
{
    char ch = sc & 0x7f;
    if (mode & E0ESC)
    {
        switch (ch)
        {
            case 0x1D: return CTRL;
            case 0x38: return ALT;
        }
    }
    else
    {
        switch (ch)
        {
            case 0x2A:
            case 0x36: return SHIFT;
            case 0x1D: return CTRL;
            case 0x38: return ALT;
        }
    }
    return 0;
}
int do_keyboard_intr(struct trap *tf)
{
    uchar sc,ch,m;
    uchar *map = keybd_map;
    if ((inb(KB_STAT) & KB_STAT_DIB) == 0) return -1;
    sc = inb(KB_DATA);
    if ((sc & 0x7f) == 0x3A) return 0;
    if (sc == 0xE0) mode |= E0ESC;
    if ((m = shift(sc)))
    {
        if (sc & 0x80) mode &= ~m;
        else mode |= m;
        return 0;
    }
    map = (mode & SHIFT) ? shift_map : keybd_map;
    ch = map[sc & 0x7f];
    if (mode & CTRL)
    {
        switch (ch)
        {
            case 'c': ch = CINTR;
            case 'd': ch = CEOF;
            case 'x': ch = CKILL;
            case 'q': ch = CSTART;
            case 's': ch = CSTOP;
            case 'z': ch = CSUSP;
            case '\\': ch = CQUIT;
        }
    }
    if (((sc & 0x80) == 0) && (ch != '\0')) tty_input(&tty[0],ch);
    else mode &= ~E0ESC;
    return 0;
}
void keybd_init()
{
    irq_enable(1);
    set_hwint(IRQ0 + 1,&do_keyboard_intr);
}
