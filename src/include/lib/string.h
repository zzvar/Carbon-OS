#ifndef __INCLUDE_LIB_STRING_H__
#define __INCLUDE_LIB_STRING_H__
#include <types.h>
void memcpy(void *dest,void *src,uint32_t len);
void memset(void *dest,uint8_t val,uint32_t len);
void bzero(void *dest,uint8_t len);
int strcmp(const char *str1,const char *str2);
char *strcpy(char *dest,const char *src);
char *strncpy(char *dest,const char *src,uint32_t len);
char *strcat(char *dest,const char *src);
int strlen(const char *src);
#endif
