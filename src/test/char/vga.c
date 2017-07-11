#include <param.h>
#include <x86.h>
#include <proto.h>
#include <tty.h>
struct vchar
{
    char vc_char:8;
    char vc_color:4;
    char vc_bgcolor:4;
};
#define VGA_WHILE 0x07
#define VGA_BLACK 0x00
#define VC_BLANK (' ' | VGA_WHITE << 8)
extern struct vchar vgamem[25][80];
#define px (tty[0].t_col)
#define py (tty[0].t_row)
void flush_csr()
{
    uint pos = py * 80 + px;
    outb(0x3D4,14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4,15);
    outb(0x3D5,pos);
}
void cls()
{
    memsetw((short*)vgamem,VC_BLANK,80 * 25);
    px = 0;
    py = 0;
    flush_csr();
}
void scroll(void)
{
    if (py >= 25)
    {
        uint pos = py - 25 + 1;
        memcpy(vgamem,&vgamem[pos][0],(25 - pos) * 80 * sizeof(struct vchar));
        memsetw((short*)&vgamem[25 - pos][0],VC_BLANK,80);
        py = 25 - 1;
    }
}
void putch(char c)
{
    if (c == '\b')
    {
        if (px > 0)
        {
            px--;
            vgamem[py][px].vc_char = ' ';
        }
        else
        {
            if (py > 0) py--;
            px = 79;
            vgamem[py][px].vc_char = ' ';
        }
    }
    else if (c == '\t') px = (px + 4) & ~3;
    else if (c == '\r') px = 0;
    else if (c == '\n')
    {
        px = 0;
        py++;
    }
    else if (c >= ' ')
    {
        vgamem[py][px].vc_char = c;
        px++;
    }
    if (px >= 80)
    {
        px = 0;
        py++;
    }
    scroll();
    flush_csr();
}
void puts(char *str)
{
    int i;
    for (i = 0;i < strlen(str);i++)
        putch(str[i]);
}
void println(char *fmt, ...)
{
    static char *ntab = "0123456789ABCDEF";
    uint a,m;
    if (a = n / b) printn(a,b);//if this line error,change to 'if ((a = n / b))'
    m = n % b;
    putch(ntab[m]);
}
void printk(char *fmt, ...)
{
    char c;
    int n;
    int *adx = (int*)(void*)&fmt + 1;
    _loop:
    while ((c = *fmt++) != '%')
    {
        if (c == '\0') return;
        putch(c);
    }
    c = *fmt++;
    if ((c == 'd') || (c == 'l'))
    {
        n = *adx;
        if (n < 0)
        {
            putch('-');
            n *= -1;
        }
        printn(n,10);
    }
    if ((c == 'o') || (c == 'x')) printn(*adx,(c == 'o' ? 8 : 16));
    if (c == 'c') putch(*adx);
    if (c == 's') puts((char*)*adx);
    adx++;
    goto _loop;
}
