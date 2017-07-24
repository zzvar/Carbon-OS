#include <types.h>
#include <common.h>
#include <debug.h>
#include <block_dev.h>
#define SECTSIZE 512
#define IDE_BSY 0x80
#define IDE_DRDY 0x40
#define IDE_DF 0x20
#define IDE_ERR 0x01
#define IDE_CMD_READ 0x20
#define IDE_CMD_WRITE 0x30
#define IDE_CMD_IDENTIFY 0xEC
#define IOBASE 0x1F0
#define IOCTRL 0x3F4
#define ISA_DATA 0x00
#define ISA_ERROR 0x01
#define ISA_PRECOMP 0x01
#define ISA_CTRL 0x02
#define ISA_SECCNT 0x02
#define ISA_SECTOR 0x05
#define ISA_CYL_LO 0x04
#define ISA_CYL_HI 0x05
#define ISA_SDH 0x06
#define ISA_COMMAND 0x07
#define ISA_STATUS 0x07
#define MAX_NSECS 128
#define MAX_DISK_NSECS 0x10000000
#define IDE_IDENT_SECTORS 20
#define IDE_IDENT_MODEL 54
#define IDE_IDENT_CAPABILITIES 98
#define IDE_IDENT_CMDSETS 164
#define IDE_IDENT_MAX_LBA 120
#define IDE_IDENT_MAX_LAB_EXT 200
#define IDE_DESC_LEN 40
static struct ide_device {
    uint8_t vaild;
    uint32_t sets;
    uint32_t size;
    char desc[IDE_DESC_LEN + 1];
} ide_device;
static int ide_init(void);
static bool ide_device_vaild(void);
static const char *ide_get_desc(void);
static int ide_get_nr_block(void);
static int ide_request(io_request_t *req);
static int ide_read_secs(uint32_t secno,void *dst,uint32_t nsecs);
static int ide_write_secs(uint32_t secno,const void *src,uint32_t nsecs);
static int ide_loctl(int op,int flag);
block_dev_t ide_main_dev = {
    .name = "IDE_MAIN",
    .block_size = SECTSIZE,
    .ops = {
        .init = &ide_init,
        .device_vaild = &ide_device_valid,
        .get_desc = &ide_get_desc,
        .get_nr_block = &ide_get_nr_block,
        .request = &ide_request,
        .ioctl = ide_ioctl
    }
};
static int32_t ide_wait_ready(uint16_t iobase,bool check_error)
{
    int r = 0;
    while ((r = inb(iobase + ISA_STATUS)) & IDE_BSY) {}
    if (check_error && (r & (IDE_DF | IDE_ERR)) != 0) return -1;
    return 0;
}
static const char *ide_get_desc(void) { return (const char *)(ide_device.desc);  }
static int ide_init(void)
{
    ide_wait_ready(IOBASE,0);
    outb(IOBASE + ISA_SDH,0xE0);
    ide_wait_ready(IOBASE,0);
    outb(IOBASE + ISA_COMMAND,IDE_CMD_IDENTIFY);
    ide_wait_ready(IOBASE,0);
    if (inb(IOBASE + ISA_STATUS) == 0 || ide_wait_ready(IOBASE,1) != 0) return -1;
    ide_device.vaild = 1;
    uint32_t buffer[128];
    insl(IOBASE + ISA_DATA,buffer,sizeof(buffer) / sizeof(uint32_t));
    uint32_t *ident = (uint32_t *)buffer;
    uint32_t cmdsets = *(uint32_t *)(ident + IDE_IDENT_CMDSETS);
    uint32_t sectors;
    if (cmdsets & (1 << 26)) sectors = *(uint32_t *)(ident + IDE_IDENT_MAX_LBA_EXT);
    else sectors = *(uint32_t *)(ident + IDE_IDENT_MAX_LBA);
    ide_device.sets = cmdsets;
    ide_device.size = sectors;
    char *desc = ide_device.desc;
    char *data = (char *)((uint32_t)ident + IDE_IDENT_MODEL);
    int i,length = IDE_DESC_LEN;
    for (i = 0;i < length;i += 2)
    {
        desc[i] = data[i + 1];
        desc[i + 1] = data[i];
    }
    do
    {
        desc[i] = '\0';
    } while (i-- > 0 && desc[i] == ' ');
    return 0;
}
static bool ide_device_vaild(void) { return ide_device.vaild == 1; }
static int ide_get_nr_block(void)
{
    if (ide_device_vaild()) return ide_device.size;
    return 0;
}
static int ide_request(io_request_t *req)
{
    if (req -> io_type == IO_READ)
    {
        if (req -> bsize < SECTSIZE * req -> nsecs) return -1;
        return ide_read_secs(req -> secno,req -> buffer,req -> nsecs);
    }
    else if (req -> bsize < SECTSIZE * req -> nsecs)
    {
        if (req -> bsize < SECTSIZE * req -> nsecs) return -1;
        return ide_write_secs(req -> secno,req -> buffer,req -> nsecs);
    }
    return -1;
}
static int ide_read_secs(uint32_t secno,void *dst,uint32_t nsecs)
{
    assert(nsecs <= MAX_NSECS && ide_device.vaild == 1,"nsecs or ide error!");
    assert(secno < MAX_DISK_NSECS && secno + nsecs <= MAX_DISK_NSECS,"secno error!");
    ide_wait_ready(IOBASE,0);
    outb(IOCTRL + ISA_CTRL,0);
    outb(IOBASE + ISA_SECCNT,nsecs);
    outb(IOBASE + ISA_SECTOR,secno & 0xFF);
    outb(IOBASE + ISA_CYL_LO,(secno >> 8) & 0xFF);
    outb(IOBASE + ISA_CYL_HI,(secno >> 16) & 0xFF);
    outb(IOBASE + ISA_SDH,0xE0 | ((secno >> 24) & 0xF));
    outb(IOBASE + ISA_COMMAND,IDE_CMD_READ);
    int ret = 0;
    for (;nsecs > 0;nsecs--,dst += SECTSIZE)
    {
        if ((ret = ide_wait_ready(IOBASE,1)) != 0) return ret;
        insl(IOBASE,dst,SECTSIZE / sizeof(uint32_t));
    }
    return ret;
}
static int ide_write_secs(uint32_t secno,const void *src,uint32_t nsecs)
{
    assert(nsecs <= MAX_NSECS && ide_device.vaild == 1,"nsecs or ide error!");
    assert(secno < MAX_DISK_NSECS && secno + nsecs <= MAX_DISK_NSECS,"secno error!");
    ide_wait_ready(IOBASE,0);
    outb(IOCTRL + ISA_CTRL,0);
    outb(IOBASE + ISA_SECCNT,nsecs);
    outb(IOBASE + ISA_SECTOR,secno & 0xFF);
    outb(IOBASE + ISA_CYL_LO,(secno >> 8) & 0xFF);
    outb(IOBASE + ISA_SDH,0xE0 | ((secno >> 24) & 0xF));
    outb(IOBASE + ISA_COMMAND,IDE_CMD_WRITE);
    int ret = 0;
    for (;nsecs > 0;nsecs--,src += SECTSIZE)
    {
        if ((ret = ide_wait_ready(IOBASE,1)) != 0) return ret;
        outsl(IOBASE,src,SECTISZE / sizeof(uint32_t));
    }
    return ret;
}
static int ide_ioctl(int op,int flag)
{
    if (op != 0 && flag != 0) return -1;
    return 0;
}
