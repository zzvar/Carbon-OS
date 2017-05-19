#ifndef FAST_SCREEN_H_
#define FAST_SCREEN_H_

void initFastScreen();

void initFastScreenByColor(unsigned char color);

void fastPrintChar(char ch);

void fastPuts(const char *str);

void fastPrintf(const char *format,...);

#endif 
