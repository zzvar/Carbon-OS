#include <param.h>
#include <x86.h>
#include <proto.h>
#include <buf.h>
#include <proc.h>
#include <conf.h>
#include <hd.h>
// hard disk part
struct devtab hdtab = {0,};
int hd_cmd(uint drive,uint cmd,uint lba,uchar ns)
{
    hd_wait_ready();
    outb(0x3F6,0);
    outb(0x1F2,ns);
    outb(0x1F3,lba & 0xFF);
    outb(0x1F4,(lba >> 8) & 0xFF);
    outb(0x1F5,(lba >> 16) & 0xFF);
    outb(0x1F6,0xE0 | ((drive & 1) << 4) | ((lba >> 24) & 0x0F));
    outb(HD_CMD,cmd);
    return 0;
}
int hd_wait_ready()
{
    int i = 10000,r;
    while(--i || ((r = inb(HD_STAT)) & HD_BSY))
    {
        
    }
    return i;
}
int hd_request(struct buf *bp)
{
    bp -> av_prev = (struct buf*)&hdtab;
    bp -> av_next = hdtab.av_next;
    hdtab.av_next -> av_prev = bp;
    hdtab.av_next = bp;
    if (hdtab.d_active == 0) hd_start();
    return 0;
}
void hd_start()
{
    struct buf *bp;
    if ((struct devtab*)hdtab.av_next == &hdtab) return;
    bp = hdtab.av_prev;
    hdtab.d_avtice = 1;
    if (bp -> b_flag & B_READ) 
        hd_cmd(0,HD_CMD_WRITE,bp -> b_blkno * BLK / PBLK,BLK / PBLK);
    else
    {
        hd_cmd(0,HD_CMD_WRITE,bp -> b_blkno * BLK / PBLK,BLK / PBLK);
        outsl(0x1F0,bp -> b_data,BLK / 4);
    }
}
int do_hd_intr(struct trap *tf)
{
    struct buf *bp;
    if (hdtab.d_active == 0) return 0;
    hdtab.d_active = 0;
    bp = hdtab.av_prev;
    bp -> av_prev -> av_next = bp -> av_next;
    bp -> av_next -> av_prev = bp -> av_prev;
    if (bp -> b_flag & B_READ) insl(0x1F0,bp -> b_data,BLK / 4);
    iodone(bp);
    hd_start();
    return 0;
}
void hd_init()
{
    irq_enable(14);
    set_hwint(IRQ0 + 14,&do_hd_intr);
}
int nulldev(){ return 0 };
int nodev()
{
    syserr(ENODEV);
    return 0;
}
