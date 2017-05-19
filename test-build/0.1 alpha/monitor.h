#ifndef _MONITOR_H_
#define _MONITOR_H_
#include "common.h"
void monitor_put(char c);
void monitor_clear();
void monitor_write(char *c);
void monitor_write_hex(u32int n);
void monitor_write_dec(u32int n);
#endif