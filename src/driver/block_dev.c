#include <debug.h>
#include <lib/string.h>
#include <block_dev.h>
block_dev_t *block_devs;
void block_dev_init(void)
{
    cprintk(rc_black,rc_red,"Init IDE Driver ...\n");
    block_dev_t *ide_dev = &ide_main_dev;
    if (ide_dev -> ops.init() == -1)
    {
        printk("Main IDE Device Not Found!\n");
        return;
    }
    add_block_dev(ide_dev);
    if (!ide_dev -> ops.device_vaild())
    {
        printk("Main IDE Device Error!\n");
        return;
    }
    cprintk(rc_black,rc_red,"Found IDE driver: %u (sectors) Desc: %s\n",ide_dev -> ops.get_nr_block(),ide_dev -> ops.get_desc());
    if (read_mbr_info(ide_dev) != 0) printk("Read MBR Info Error!");
}
int add_block_dev(block_dev_t *bdev)
{
    block_dev_t *p = block_devs;
    while (p)
    {
        if (strcmp(p -> name,bdev -> name) == 0) return -1;
        p = p -> next;
    }
    bdev -> next = block_devs;
    block_devs = bdev;
    return 0;
}
