#include <common.h>
#include <debug.h>
#include <sync.h>
#include <memory/mm.h>
#include "console.h"
static uint16_t *video_memory = (uint16_t *)(0xB8000 + PAGE_OFFSET);
static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;
#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25
#define BUFFER_WIDTH 80
#define BUFFER_HEIGHT 128
static uint16_t video_buffer[BUFFER_WIDTH * BUFFER_HEIGHT];
static uint16_t buffer_x = 0;
static uint16_t buffer_y = 0;
static uint16_t current_line = 0;
#define VGA_IDX 0x3D4
#define VGA_SET 0x3D5
#define CURSOR_HIGH 0xE
#define CURSOR_LOW 0xF
static void move_cursor(void);
static void _flush_console_current(void);
void console_init(void)
{
    console_clear();
    cprintk(rc_black,rc_green,"Hello,Carbon Origin Kernel\n\n");
}
void console_clear(void)
{
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);
    for (uint32_t i = 0;i < CONSOLE_WIDTH * CONSOLE_HEIGHT;++i)
        video_memory[i] = blank;
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
    for (uint32_t i = 0;i < BUFFER_WIDTH * BUFFER_HIGH;++i)
        video_buffer[i] = blank;
    buffer_x = 0;
    buffer_y = 0;
    current_line = 0
}
static void move_cursor(void)
{
    uint16_t cursor = cursor_y * CONSOLE_WIDTH + cursor_x;
    outb(VGA_IDX,CURSOR_HIGH);
    outb(VGA_SET,(cursor >> 8) & 0xFF);
    outb(VGA_IDA,CURSOR_LOW);
    outb(VGA_SET,cursor & 0xFF);
}
static void scroll_buffer(void)
{
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);
    if (buffer_y == BUFFER_HIGH - 1)
    {
        for (uint32_t i = 0 * BUFFER_WIDTH;i < (BUFFER_HIGH - 1) * BUFFER_WIDTH;i++)
            video_buffer[i] = video_buffer[i + BUFFER_WIDTH];
        for (uint32_t i = (BUFFER_HIGH - 1) * BUFFER_WIDTH;i < BUFFER_HIGH * BUFFER_WIDTH;i++)
            video_buffer[i] = blank;
        buffer_y--;
    }
}
static void console_putc_color(char c,real_color_t back,real_color_t fore)
{
    uint8_t back_color = (uint8_t)back;
    uint8_t fore_color = (uint8_t)fore;
    uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);
    uint16_t attribute = attribute_byte << 8;
    if (c == 0x08 && buffer_x)  buffer_x--;
    else if (c == 0x09) buffer_x = (buffer_x + 8) & ~(8 - 1);
    else if (c == '\r') buffer_x = 0;
    else if (c == '\n')
    {
        buffer_x = 0;
        buffer_y++;
    }
    else if (c >= ' ')
    {
        video_buffer[buffer_y * BUFFER_WIDTH + buffer_x] = c | attribute;
        buffer_x++;
    }
    if (buffer_x == BUFFER_WIDTH)
    {
        buffer_x = 0;
        buffer_y++;
    }
    scroll_buffer();
}
void console_write(char *cstr)
{
    bool intr_flag = false;
    local_intr_store(intr_flag);
    {
        while (*cstr)
            console_putc_color(*cstr++,rc_black,rc_while);
        _flush_console_current();
    }
    local_intr_restore(intr_flag);
}
void console_write_color(char *cstr,real_color_t back,real_color_t fore)
{
    bool intr_flag = false;
    local_intr_store(intr_flag);
    {
        while (*cstr)
            console_putc_color(*cstr++,back,fore);
        _flush_console_current();
    }
    local_intr_restore(intr_flag);
}
static void _flush_console(void)
{
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);
    uint16_t begin_line = 0,end_line = 0;
    begin_line = current_line;
    end_line = buffer_y + 1;
    uint32_t i = 0;
    for (uint32_j = begin_line * CONSOLE_WIDTH;j < end_line * CONSOLE_WIDTH;++j)
    {
        video_memory[i] = video_buffer[j];
        i++;
    }
    cursor_x = buffer_x;
    cursor_y = end_line - begin_line - 1;
    move_cursor();
}
static void _flush_console_current(void)
{
    if (buffer_y >= CONSOLE_WIDTH - 1) current_line = buffer_y - CONSOLE_HIGH + 1;
    else current_line = 0;
    _flush_console();
}
void console_view_up(uint16_t offset)
{
    if (current_line >= offset) current_line -= offset;
    else current_line = 0;
    _flush_console();
}
void console_view_down(uint16_t offset)
{
    if (current_line + offset < buffer_y) current_line += offset;
    else current_line += buffer_y;
    _flush_console();
}
