/**
 * create on 2017/3/21
 * by wangchen11
 * [Carbon_OS team]
 */

#include <fastscreen.h>
#include <format.h>
#include <klog.h>

void klog(int level,const char *tag,const char *format,...)
{
	switch(level)
	{
		case KLOG_LEVEL_I:
			fastPrintChar('I');
			break;
		case KLOG_LEVEL_V:
			fastPrintChar('V');
			break;
		case KLOG_LEVEL_D:
			fastPrintChar('D');
			break;
		case KLOG_LEVEL_W:
			fastPrintChar('W');
			break;
		case KLOG_LEVEL_E:
			fastPrintChar('E');
			break;
	}
	fastPrintChar('/');
	fastPrintf("%s:",tag);
	
	unsigned char *basePtr = (unsigned char *)&format+sizeof(const char *);
	char buf[MAX_FORMAT_SIZE+1];
	
	while(format[0]!=0)
	{
		if(format[0]=='%')
		{
			int usedChar = 0;
			int bytes = formatByStr(buf,format+1,(void*)(basePtr),&usedChar);
			basePtr += bytes;
			format  += usedChar;
			if(bytes!=0)
			{
				fastPuts(buf);
			}
		}
		else
		{
			fastPrintChar(format[0]);
		}
		format++;
	}
	fastPrintChar('\n');
}



