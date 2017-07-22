#include <fs.h>
#include <device.h>
void vfs_init(void);
void sfs_init(void);
void fs_init(void)
{
    vfs_init();
    device_init();
    sfs_init();
}
