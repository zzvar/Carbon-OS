#include <common.h>
#include <intr/intr.h>
#include <char_dev.h>
#define CONTROL 0x1
#define ALT 0x2
#define ALTGR 0x4
#define LSHIFT 0x8
#define RSHIFT 0x10
#define CAPSLOCK 0x20
#define SCROLLOCK 0x40
#define NUMLOCK 0x80
#define RELEASED_MASK 0x80
typedef uint8_t key_status_t;
typedef struct keymap {
    uint8_t scancodes[128];
    uint8_t capslock_scancodes[128];
    uint8_t shift_scancodes[128];
    uint8_t control_map[8];
    key_status_t controls;
} keymap_t;
static keymap_t us_keymap = {
    {
        0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
        0,'a','s','d','f','g','h','j','k','l',';','\'','`',
        0,'\\','z','x','c','v','b','n','m',',','.','/',0,
        '*',
        0,
        ' ',
        0,
        0,0,0,0,0,0,0,0,0,0,
        0,
        0,
        0,
        72,
        0,
        '-',
        0,
        0,
        0,
        '+',
        0,
        80,
        0,
        0,
        0,
        0,0,0,
        0,
        0,
        0,
    },
    {
        0,27,'1','2','3','4','6','7','8','9','0','-','=','\b',
        '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
        0,'A','S','D','F','G','H','J','K','L',';','\'','`',
        0,'|','Z','X','C','V','B','N','M',',','.','/',0,'*',
        0,
        ' ',
        0,
        0,0,0,0,0,0,0,0,0,0,
        0,
        0,
        0,
        0,
        0,
        '-',
        0,
        0,
        0,
        '+',
        0,
        0,
        0,
        0,
        0,
        0,0,0,
        0,
        0,
        0,
    },
    {
        0,27,'!','@','#','$','%','^','&','*','(',')','_','+','\b',
        '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
        0,'A','S','D','F','G','H','J','K','L',':','"','~',
        0,'|','Z','X','C','V','B','N','M','<','>','?',0,'*',
        0,
        ' ',
        0,
        0,0,0,0,0,0,0,0,0,0,
        0,
        0,
        0,
        0,
        0,
        '-',
        0,
        0,
        0,
        '+',
        0,
        0,
        0,
        0,
        0,
        0,0,0,
        0,
        0,
        0,
    },
    {
        29,
        56,
        0,
        42,
        54,
        58,
        70,
        69
    },
    0
};
static int keyboard_init(void);
static bool keyboard_device_vaild(void);
static const char *keyboard_get_desc(void);
static int keyboard_read(void *dec,uint32_t len);
static int keyboard_write(const void *src,uint32_t len);
static int keyboard_ioctl(int op,int flag);
static void keyboard_handler(__UNUSED__ pt_regs_t *regs);
char_dev_t keyboard_dev = {
    .name = "Keyboard",
    .is_readable = true,
    .is_writeable = false,
    .ops = {
        .init = keyboard_init,
        .device_vaild = keyboard_device_vaild,
        .get_desc = keyboard_get_desc,
        .read = keyboard_read,
        .write = keyboard_write,
        .ioctl = keyboard_ioctl
    }
};
#define KBUFFER_LEN 1024
static struct keyboard_device {
    bool is_vaild;
    keymap_t *curr_layout;
    uint8_t kbuffer[KBUFFER_LEN];
    uint32_t kfront;
    uint32_t krear;
} keyboard_device;
static int keyboard_init(void)
{
    keyboard_device.curr_layout = &us_keymap;
    keyboard_device.is_vaild = true;
    register_interrupt_handler(IRQ1,&keyboard_handler);
    return 0;
}
static bool keyboard_device_vaild(void) { return keyboard_device.is_vaild; }
static const char *keyboard_get_desc(void) { return "Keyboard"; }
static uint8_t keyboard_getchar(void)
{
    if (keyboard_device.kfront != keyboard_device.krear)
    {
        char ch = keyboard_device.kbuffer[keyboard_device.kfront];
        keyboard_device.kfront = (keyboard_device.kfront + 1) % KBUFFER_LEN;
        return ch;
    }
    else
    {
        return 0;
    }
}
static int keyboard_read(void *dec,uint32_t len)
{
    uint8_t *buffer = (uint8_t *)dec;
    uint8_t ch = 0;
    uint32_t i = 0;
    while (i < len)
    {
        if ((ch = keyboard_getchar()) != 0)
        {
            buffer[i] = ch;
            i++;
        }
        else
        {
            break;
        }
    }
    return i;
}
static int keyboard_write(__UNUSED__ const void *src,__UNUSED__ uint32_t len) { return -1; }
static void keyboard_handler(__UNUSED__ pt_regs_t *regs)
{
    uint8_t scancode = inb(0x60);
    keymap_t *layout = keyboard_device.curr_layout;
    if (scancode & RELEASED_MASK)
    {
        uint32_t i;
        for (i = 0;i < 5;i++)
        {
            if (layout -> control_map[i] == (scancode & ~RELEASED_MASK))
            {
                layout -> controls &= ~(1 << i);
                return;
            }
        }
    }
    else
    {
        uint32_t i;
        for (i = 0;i < 8;i++)
        {
            if (layout -> control_map[i] == scancode)
            {
                if (layout -> controls & 1 << i) layout -> controls &= ~(1 << i);
                else layout -> controls |= (1 << i);
                return;
            }
        }
        uint8_t *scancodes = layout -> scancodes;
        if ((layout -> controls & (LSHIFT | RSHIFT)) && !(layout -> controls & CONTROL)) scancodes = layout -> shift_scancodes;
        if ((layout -> controls & (CAPSLOCK)) && !(layout -> controls & CONTROL)) scancodes = layout -> capslock_scancodes;
        if (keyboard_device.kfront != (keyboard_device.krear + 1) % KBUFFER_LEN)        {
            keyboard_device.kbuffer[keyboard_device.krear] = scancodes[scancode];
            keyboard_device.krear = (keyboard_device.krear + 1) % KBUFFER_LEN;
        }
    }
}
static int keyboard_ioctl(int op,int flag)
{
    if (op != 0 && flag != 0) return -1;
    return 0;
}
