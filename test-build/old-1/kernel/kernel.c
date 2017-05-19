/**
 * create on 2017/3/20
 * by wangchen11
 * [Carbon_OS team]
 */

#include <fastscreen.h>
#include <memory.h>
#include <LinkedList.h>
#include <klog.h>

#define LOG_TAG "kernel"

int kernel_main()
{
	initFastScreen();
	initMemory();
	KLOGD("before testLinkedList:memory used/total:%x/%x",memoryUsed(),memoryTotal());
	testLinkedList();
	KLOGD("after  testLinkedList:memory used/total:%x/%x",memoryUsed(),memoryTotal());
	
	return 0;
}