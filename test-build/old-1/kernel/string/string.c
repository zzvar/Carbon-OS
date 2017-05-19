/**
 * create on 2017/3/20
 * by wangchen11
 * [Carbon_OS team]
 */
#include "string.h"

int strcmp(const char *s1,const char *s2)
{
	while(s1[0]&&s2[0])
	{
		if(s1[0]!=s2[0])
			return s1[0]-s2[0];
		s1++;
		s2++;
	}
	return 0;
}

int strncmp(const char *s1,const char *s2,int n)
{
	int i=0;
	for(i=0;i<n;i++)
	{
		if(s1[i]!=s2[i])
			return s1[i]-s2[i];
	}
	return 0;
}

int strlen(const char *str)
{
	int i=0;
	for(i=0;str[i]!=0;i++)
	{
	}
	return i;
}

char* strncpy(char *dst,const char *src,int n)
{
	int i=0;
	for(i=0;(src[i]!=0)&&i<n;i++)
	{
		dst[i]=src[i];
	}
	dst[i]=0;
	return dst+i;
}

char* strcpy(char *dst,const char *src)
{
	int i=0;
	for(i=0;(src[i]!=0);i++)
	{
		dst[i]=src[i];
	}
	dst[i]=0;
	return dst+i;
}


void *memcpy(void *s1,const void *s2,int n)
{
	char *dst=(char*)s1;
	const char *src=(const char*)s2;
	int i=0;
	for(i=0;i<n;i++)
	{
		dst[i]=src[i];
	}
	return dst+i;
}
