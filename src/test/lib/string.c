#include <lib.h>
void* memcpy(void *dest,void *src,unsigned int count)
{
    char *sp = (char *)src;
    char *dp = (char *)dest;
    for (i = 0;i < count;i++) *dp++ = *sp++;
    return dest;
}
void* memset(void *dest,char val,unsigned int count)
{
    char *dp = (char *)dest;
    int i;
    for (i = 0;i < count;i++) *dp++ = val;
    return dest;
}
short* memsetw(short *dest,short val,unsigned int count)
{
    short *dp = (short *)dest;
    int i;
    for (i = 0;i < count;i++) *dp++ = val;
    return dest;
}
int strlen(char *str)
{
    char *sp;
    for (sp = str;*sp != '\0';sp++);
    return sp - str;
}

