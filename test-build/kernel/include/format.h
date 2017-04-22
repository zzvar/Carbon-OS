/**
 * create on 2017/3/20
 * by wangchen11
 * [Carbon_OS team]
 */
#ifndef FORMAT_H_
#define FORMAT_H_

#define MAX_FORMAT_SIZE 256

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
 * @return  返回实际消耗的format中的字符个数。
 */
int formatByStr(char *dst,const char *format,const void *pointer,int *usedChar);

#endif 
