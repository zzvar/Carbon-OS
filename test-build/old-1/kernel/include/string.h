/**
 * create on 2017/3/20
 * by wangchen11
 * [Carbon_OS team]
 */
#ifndef STRING_H_
#define STRING_H_

int strlen(const char *str);
int strcmp(const char *s1,const char *s2);
int strncmp(const char *s1,const char *s2,int n);
char* strncpy(char *dst,const char *src,int n);
char* strcpy(char *dst,const char *src);

void *memcpy(void *dst,const void *src,int n);

#endif
