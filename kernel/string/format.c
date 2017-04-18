/**
 * create on 2017/3/20
 * by wangchen11
 * [Carbon_OS team]
 */
#include "format.h"
#include "string.h"

static void formatChar(char *dst,char var)
{
	dst[0] = var;
	dst[1] = 0;
}

static void formatIntEx(char *dst,unsigned int var,int *pos)
{
	if(var==0)
		return;
	formatIntEx(dst,var/10,pos);
	dst[*pos]=var%10+'0';
	(*pos)++;
}

static void formatInt(char *dst,int var)
{
	int pos = 0;
	if(var&0x80000000)//var<0
	{
		dst[pos]='-';
		pos++;
		formatIntEx(dst,-var,&pos);
		if(pos==1)
		{
			dst[pos]='0';
			pos++;
		}
		dst[pos]=0;
	}
	else
	{
		formatIntEx(dst,var,&pos);
		if(pos==0)
		{
			dst[pos]='0';
			pos++;
		}
		dst[pos]=0;
	}
}

static char getHexChar(int var)
{
	if( (var>=0)&&(var<=9) )
		return var+'0';
	if( (var>=10)&&(var<=15) )
		return var-10+'a';
	return '?';
}

static void formatHexIntEx(char *dst,unsigned int var,int *pos)
{
	if(var==0)
		return;
	formatHexIntEx(dst,var>>4,pos);
	dst[*pos]=getHexChar(var&0xf);
	(*pos)++;
}

static void formatHexInt(char *dst,unsigned int var)
{
	int pos = 0;
	formatHexIntEx(dst,var,&pos);
	if(pos==0)
	{
		dst[pos]='0';
		pos++;
	}
	dst[pos]=0;
}


/*
not used!
static void formatFloat(char *dst,float var)
{
	//整数部分 
	int integer = (int)var;
	//小数部分 
	float decimal = var-integer;
	formatInt(dst,integer);
	integer = (decimal*10000000);
	if(integer<0)
		integer=-integer;
	while( integer%10==0 )
	{
		integer/=10;
	}
	int pos = strlen(dst);
	dst[pos]='.';
	dst[pos+1]=0;
	formatInt(dst+strlen(dst),integer);
}

static void formatDouble(char *dst,double var)
{
	
}
*/

static int min(int a,int b)
{
	return a<b?a:b;
}

/**
 * 将pointer以format形式解析到dst中。
 * 例如:
 * char buf[MAX_FORMAT_SIZE];
 * int a=100;
 * formatByStr(buf,"d",&a);
 * @dst     目标缓冲区。
 * @format  格式,int:d float:f 等等。
 * @pointer 指向需要被解析的数据。
 * @usedChar 返回消耗的字符数。
 * @return  返回实际解析到的数据所占的字节数,0解析失败。
 */
int formatByStr(char *dst,const char *format,const void *pointer,int *usedChar)
{
	dst[0]=0;
	*usedChar=0;
	if( strncmp(format,"c",1)==0 )
	{
		formatChar(dst,((char*)pointer)[0]);
		*usedChar=1;
		return 4;
	}
	if( strncmp(format,"d",1)==0 )
	{
		formatInt(dst,((int*)pointer)[0]);
		*usedChar=1;
		return 4;
	}
	if( strncmp(format,"s",1)==0 )
	{
		char *src = ((char**)pointer)[0];
		strncpy(dst,src, min(MAX_FORMAT_SIZE,strlen(src) )) [0]=0;
		*usedChar=1;
		return 4;
	}
	if( strncmp(format,"x",1)==0 )
	{
		formatHexInt(dst,((unsigned int*)pointer)[0]);
		*usedChar=1;
		return 4;
	}
	if( strncmp(format,"p",1)==0 )
	{
		dst[0]='0';
		dst[1]='x';
		formatHexInt(dst+2,((unsigned int*)pointer)[0]);
		*usedChar=1;
		return 4;
	}
	return 0;
}



