/**
 * create on 2017/3/21
 * by wangchen11
 * [Carbon_OS team]
 */
#include <multboot.h>

/**
 * 初始化内存分配
 * 初始化之后我们就能使用malloc了！
 * 是不是很激动！！！！
 */
int initMemory();

/**
 * 分配内存
 * 在用完这段内存后记得调用free函数释放内存。
 * 记住，一定不要写越界，否则很容易导致内存分配链崩溃，然后系统跑飞。
 */
void* malloc(uint32_t size);

/**
 * 释放由malloc申请的内存
 */
void free(void *mem);

/**
 * 获取剩余内存
 */
uint32_t memoryFree();

/**
 * 获取已用内存
 */
uint32_t memoryUsed();

/**
 * 获取最大内存
 */
uint32_t memoryTotal();
