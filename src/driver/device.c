#include <device.h>
#include <char_dev.h>
#include <block_dev.h>
void device_init(void)
{
    char_dev_init();
    block_dev_init();
}
