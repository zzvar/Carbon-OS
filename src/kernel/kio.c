#include <common.h>
#include <char_dev.h>
#include <kio.h>
char getchar(void)
{
    char ch;
    char_dev_t *kb_dev = &keyboard_dev;
    if (!kb_dev -> ops.device_vaild()) return 0;
    while (kb_dev -> ops.read(&ch,1) == 1)
    {
        cpu_init();
        return ch;
    }
    return 0;
}
