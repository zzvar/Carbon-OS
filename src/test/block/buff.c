#include <param.h>
#include <x86.h>
#include <proto.h>
#include <proc.h>
#include <buf.h>
#include <conf.h>
// buffer's constant variable and struct
char buffers[NBUF][BLK] __attribute__((aligned(BLK)));
struct buf buff[NBUF];
struct buf bfreelist;
int nblkdev = 0;
// Now,define some important function
struct buf* incore(ushort dev,uint blkno)
{
    struct devtab *dtp;
    struct buf *bp;
    dtp = bdevsw[MAJOR(dev)].d_tab;
    for (bp = dtp -> b_next;bp != (struct buf*)dtp;bp = bp -> b_next)
        if ((bp -> b_dev == dev) && (bp -> b_blkno == blkno)) return bp;
    return NULL;
}
struct buf* getblk(int dev,uint blkno)
{
    struct buf *bp;
    struct devtab *dtp;
    if (MAJOR(dev) >= nblkdev) panic("Error devno.");
    // if the downest if statement have error
    // the program come back to here
    // else continue to run
    _loop:
    if (dev < 0) dtp = (struct devtab *) &bfreelist;
    else
    {
        dtp = bdevsw[MAJOR(dev)].d_tab;
        if (dtp == NULL) panic("Error devtab");
        if ((bp = incore(dev,blkno)))
        {
            cli();
            if (bp -> b_flag & B_BUSY)
            {
                bp -> b_flag |= B_WANTED;
                sleep((uint)bp,PRIBIO);
                sti();
                goto _loop;
            }
            sti();
            notavail(bp);
            return bp;
        }
    }
    cli();
    if (bfreelist.av_next == &bfreelist)
    {
        bfreelist.b_flag |= B_WANTED;
        sleep((uint)&bfreelist,PRIBIO);
        sti();
        goto _loop;
    }
    sti();
    bp = bfreelist.av_next;
    notavail(bp);
    bp -> b_flag = B_BUSY;
    if (bp -> b_flag & B_DIRTY)
    {
        bp -> b_flag |= B_ASYNC;
        bwrite(bp);
        goto _loop;
    }
    cli();
    bp -> b_prev -> b_next = bp -> b_next;
    bp -> b_next -> b_prev = bp -> b_prev;
    bp -> b_prev = (struct buf*)dtp;
    bp -> b_next = dtp -> b_next;
    dtp -> b_next -> b_prev = bp;
    dtp -> b_next = bp;
    sti();
    bp -> b_dev = dev;
    bp -> b_blkno = blkno;
    return bp;
}
void brelse(struct buf *bp)
{
    if (bp -> b_flag & B_WANTED) wakeup((uint)bp);
    if (bfreelist.b_flag & B_WANYED)
    {
        bfreelist.b_flag &= ~B_WANTED;
        wakeup((uint)&bfreelist);
    }
    if (bp -> b_flag & B_ERROR) bp -> b_dev = NODEV;
    bp -> b_flag &= ~(B_WANTED | B_BUSY | B_DIRTY | B_ASYNC);
    bp -> av_next = &bfreelist;
    bp -> av_prev = bfreelist.av_prev;
    bp -> ac_prev -> av_next = bp;
    bp -> av_next -> av_prev = bp;
}
void notavail(struct buf *bp)
{
    bp -> av_prev -> av_next = bp -> av_next;
    bp -> av_next -> av_prev = bp -> av_prev;
    bp -> b_flag |= B_BUSY;
}
void iowait(struct buf *bp)
{
    while((bp -> b_flag & B_DONE) == 0) sleep((uint)bp,PRIBIO);
}
void iodone(struct buf *bp)
{
    bp -> b_flag |= B_DONE;
    bp -> b_flag &= ~B_WANTED;
    wakeup((uint)bp);
}
struct buf* bread(int dev,uint blkno)
{
    struct buf *bp;
    struct getblk(dev,blkno);
    if (bp -> b_flag & D_DONE) return bp;
    bp -> b_flag |= B_READ;
    (*bdevsw[MAJOR(dev)].d_request)(bp);
    iowait(bp);
    return bp;
}
void bwrite(struct buf *bp)
{
    uint flag;
    flag = bp -> b_flag;
    bp -> b_flag &= ~(B_READ | B_DONE | B_ERROR | B_DIRTY);
    (*bdevsw[MAJOR(bp -> b_dev)].d_request)(bp);
    if ((flag & B_ASYNC) == 0) iowait(bp);
}
void buf_init()
{
    uint i = 0;
    struct buf *bp;
    struct bdevsw *bsp;
    struct devtab *dtp;
    bfreelist.b_prev = bfreelist.b_next = &bfreelist;
    bfreelist.av_prev = bfreelist.av_next = &bfreelist;
    for (i = 0;i < NBUF;i++)
    {
        bp = &buff[i];
        bp -> b_dev = NODEV;
        bp -> b_data = buffers[i];
        bp -> b_flag = B_BUSY;
        bp -> b_next = bp -> b_prev = bp;
        brelse(bp);
    }
    nblkdev = 0;
    for (bsp = &bdevsw[0];bsp < &bdevsw[NBLKDEV];bsp++)
    {
        dtp = bsp -> d_tab;
        dtp -> b_next = dtp -> b_prev = (struct buf *) dtp;
        dtp -> av_next = dtp -> av_prev = (struct buf *) dtp;
        nblkdev++;
    }
}
void dump_buf(struct buf *bp)
{
    int i,c;
    for (i = 0;i < 255;i++)
    {
        c = bp -> b_data[i];
        if (c > 31 && c < 127) printk("%c",c);
        else printk("%x",c);
    }
    printk("\n");
}
