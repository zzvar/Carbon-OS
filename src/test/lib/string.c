#include <lib.h>
void* memcpy(void *dest,void *src,unsigned int count)
{
    char *sp = (char *)src;
    char *dp = (char *)dest;
    for (int i = 0;i < count;i++) *dp++ = *sp++;
    return dest;
}
void* memset(void *dest,char val,unsigned int count)
{
    char *dp = (char *)dest;
    for (int i = 0;i < count;i++) *dp++ = val;
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
int strnlen(char *str,unsigned int len)
{
    char *sp;
    for (sp = str;(*sp != '\0') && (sp <= str + len);sp++);
    return sp - str;
}
char* strcpy(char *dst,const char *src)
{
    char *tmp = dst;
    while (*dst++ = *src++); // or while ((*dst++ = *src++))
        return tmp;
}
char *strncpy(char *dst,const char *src,unsigned int cnt)
{
    char *tmp = dst;
    while (cnt && (*dst++ = *src++)) cnt--;
    if (cnt > 0)
        while (--cnt) *dst++ = '\0';
    return tmp;
}
char* strchr(const char *str,int c)
{
    char *tmp = (char*)str;
    for (;*tmp != '\0';tmp++)
        if (*tmp == (char)c) return tmp;
    if (*tmp == (char)c) return tmp;
    return NULL;
}
char* strrchr(const char *str,int c)
{
    char *ret,*tmp;
    for (;tmp != '\0';tmp++)
        if (*tmp == (char)c) ret = tmp;
    if (*ret == (char)c) return ret;
    return NULL;
}
int strcmp(char *s1,char *s2)
{
    while (*s1 == *s2++)
        if (*s1++ == 0) return (0);
    return (*(unsigned char*)s1 - *(unsigned char*)(s2 - 1));
}
int strncmp(char *s1,char *s2,unsigned int n)
{
    if (n == 0) return 0;
    while (n-- != 0)
    {
        if (*s1 != *s2++) return (*(unsigned char*)s1 - *(unsigned char*)(s2 - 1));
        if (*s1++ == 0) break;
    }
    return 0;
}
