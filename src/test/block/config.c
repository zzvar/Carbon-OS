#include <param.h>
#include <x86.h>
#include <proto.h>
#include <proc.h>
#include <buf.h>
#include <tty.h>
#include <conf.h>
#include <hd.h>
ushort rootdev = DEVNO(1,0);
struct bdevsw bdevsw[NBLKDEV] = {
    {0,},
    {&nulldev,&nulldev,&hd_requset,&hdtab}
};
struct cdevsw cdevsw[NBLKDEV] = {
    {0,},
    {&tty_open,(void*)&nulldev,&tty_read,&tty_size,(void*)&nulldev}
};
