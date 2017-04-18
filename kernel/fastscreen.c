/**
 * create on 2017/3/20
 * by wangchen11
 * [Carbon_OS team]
 */

#include "fastscreen.h"
#include "format.h"

static const unsigned short *mFastScreenBuf = (unsigned short *)0xb8000;
static const int mFastScreenRow = 25;
static const int mFastScreenColumn = 80;
static unsigned char mFastScreenCharColor = 0x0f;
static int mNowX = 0;
static int mNowY = 0;

struct CharAndColor_st {
	unsigned char ch;
	unsigned char color;
};
typedef struct CharAndColor_st CharAndColor;


//////////////////////////////////////////////////
//All local function declaration here.
//////////////////////////////////////////////////
static void initFastScreenCharAndColor(unsigned char color);


void initFastScreen()
{
	initFastScreenCharAndColor(mFastScreenCharColor);
}

void initFastScreenByColor(unsigned char color)
{
	initFastScreenCharAndColor(color);
}

static CharAndColor* getCharPointer(int x,int y)
{
	if(x<0||y<0||x>=mFastScreenColumn||y>=mFastScreenRow)
		return (void*)0;
	CharAndColor *cac = (CharAndColor *) mFastScreenBuf + x + y*mFastScreenColumn;
	return cac;
}

static void initFastScreenCharAndColor(unsigned char color)
{
	int x,y;
	for(y=0;y<mFastScreenRow;y++)
	{
		for(x=0;x<mFastScreenColumn;x++)
		{
			CharAndColor *cac = getCharPointer(x,y);
			cac->ch=' ';
			cac->color=color;
		}
	}
	mFastScreenCharColor = color;
	mNowX = 0;
	mNowY = 0;
}

static void memcpy_h2e(void *dst,const void *src,int len)
{
	int i=0;
	unsigned char *d = (unsigned char *)dst;
	const unsigned char *s = (const unsigned char *)src;
	for(i=0;i<len;i++)
	{
		d[i]=s[i];
	}
}

static void setCharAndColorAt(int x,int y,unsigned char ch,unsigned char color)
{
	getCharPointer(x,y)->ch = ch;
	getCharPointer(x,y)->color = mFastScreenCharColor;
}

static void scrollScreen()
{
	memcpy_h2e(
		(void*)mFastScreenBuf,
		(void*)(mFastScreenBuf+mFastScreenColumn),
		sizeof(unsigned short)*((mFastScreenRow-1)*mFastScreenColumn));
	int x;
	for(x=0;x<mFastScreenColumn;x++)
	{
		setCharAndColorAt(x,mFastScreenRow-1,' ',mFastScreenCharColor);
	}
	mNowY=mFastScreenRow-1;
}

void fastPrintChar(char ch)
{
	if(ch=='\t')
	{
		do{
			fastPrintChar(' ');
		}while( (mNowX%4)!=0 );
		return;
	}
	if(ch == '\r')
		return;
	if(ch == '\n')
	{
		mNowX=0;
		mNowY++;
		if(mNowY>=mFastScreenRow)
		{
			scrollScreen();
		}
		return;
	}
	setCharAndColorAt(mNowX,mNowY,ch,mFastScreenCharColor);
	mNowX++;
	if(mNowX>=mFastScreenColumn)
	{
		mNowX=0;
		mNowY++;
		if(mNowY>=mFastScreenRow)
		{
			scrollScreen();
		}
	}
}

void fastPuts(const char *str)
{
	while(str[0]!=0)
	{
		fastPrintChar(str[0]);
		str++;
	}
}

void fastPrintf(const char *format,...)
{
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
}
