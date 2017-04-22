/**
 * create on 2017/3/21
 * by wangchen11
 * [Carbon_OS team]
 */
#include <memory.h>
#include <fastscreen.h>
#include <types.h>
#include <klog.h>
#include <string.h>

#define LOG_TAG "memory"

struct MemoryNodeHead_st
{
	uint32_t mCheckSum;//校验码
	uint32_t mLength;
	struct   MemoryNodeHead_st* mProNode;  // 前一个内存节点的首地址
	struct   MemoryNodeHead_st* mNextNode; // 下一个内存节点的首地址
	//... data flow here 
};
typedef struct MemoryNodeHead_st MemoryNodeHead;

struct MemoryAllocater_st
{
	uint32_t mStartAddr;
	uint32_t mEndAddr;
	uint32_t mLength;
	uint32_t mMemoryUsed;
	MemoryNodeHead *mHead;
};
typedef struct MemoryAllocater_st MemoryAllocater;

///////////////////////////copy static function head to here//////////////////////////////////
static void createMemoryPage(int addr,int len,MemoryAllocater *allocater);
static void setCheckSum(MemoryNodeHead *head);
static int  checkSum(MemoryNodeHead *head);
static uint32_t getCheckSum(MemoryNodeHead *head);
static void dumpMemoryNodeHead(MemoryNodeHead *head);

//////////////////////////////////////////////////////////////////////////////////////////////

/**
 * 用于存储主要的内存分配器。（就是最大的那块RAM）。
 */
static MemoryAllocater mMainMemoryPage;

static void testMemory();
/**
 * 初始化内存分配
 * 初始化之后我们就能使用malloc了！
 * 是不是很激动！！！！
 */
int initMemory()
{
	//KLOGD("###########################memory###########################");
	
	uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
	uint32_t mmap_length = glb_mboot_ptr->mmap_length;

	mmap_entry_t *mmap = (mmap_entry_t *) mmap_addr;
	for (mmap = (mmap_entry_t *) mmap_addr;
			(uint32_t) mmap < mmap_addr + mmap_length; mmap++)
	{
		KLOGD("base_addr_low:%p\t:%p\t:%p\t:%d",mmap->base_addr_low,mmap->length_low,mmap->base_addr_low+mmap->length_low,mmap->type);
	}
	
	KLOGD("############################################################");
	KLOGD("#####################find max RAM page######################");
	
	mmap_entry_t *maxPage = (void*)0;
	
	mmap = (mmap_entry_t *) mmap_addr;
	for (mmap = (mmap_entry_t *) mmap_addr;
			(uint32_t) mmap < mmap_addr + mmap_length; mmap++)
	{
		if(mmap->type == 1)// 1 for RAM 
		if(  (maxPage==(void*)0) || (mmap->length_low > maxPage->length_low) )
		{
			maxPage = mmap;
		}
	}
	if(maxPage==(void*)0)
	{
		KLOGE("can't find RAM page");
		return -1;
	}
	KLOGD("max RAM page is:");
	KLOGD("addr:%p\tlen:%p\tsize:%dKB",maxPage->base_addr_low,maxPage->length_low,maxPage->length_low>>10);
	// 前256K 留给kernel rom. 
	int keepKernelSize = (256*1024);
	createMemoryPage(maxPage->base_addr_low+keepKernelSize,maxPage->length_low-keepKernelSize,&mMainMemoryPage);
	KLOGD("############################################################");
	//testMemory();
	return 0;
}

/**
 * 通过首地址和长度创建一个内存分配器。
 */
static void createMemoryPage(int addr,int len,MemoryAllocater *allocater)
{
	allocater->mStartAddr = addr;
	allocater->mLength    = len;
	allocater->mEndAddr   = addr+len;
	/**
	 * 跳过前1k，不知道从哪里开始可以写，反正前8个字节不可写。
	 */
	allocater->mHead      = (MemoryNodeHead*)addr; 
	
	allocater->mHead->mLength    = 0; //头节点不存数据 
	allocater->mHead->mProNode   = NULL;
	allocater->mHead->mNextNode  = NULL;
	setCheckSum(allocater->mHead);
}

static void dumpMemoryNodeHead(MemoryNodeHead *head)
{
	KLOGD("MemoryNodeHead:%x,%x,%x,%x",head->mCheckSum,head->mLength,head->mProNode,head->mNextNode);
}

/**
 * 通过地址计算校验码，
 * 校验内存是否为合法地址以便查错。
 */
static uint32_t getCheckSum(MemoryNodeHead *head)
{
	uint32_t check=0;
	check |= 'm'<<24;
	check |= 'e'<<16;
	check |= 'm'<<8;
	check |= 'a'<<0;
	check ^= (uint32_t)head->mLength   <<8;
	check ^= (uint32_t)(head->mProNode)  ;
	check ^= (uint32_t)(head->mNextNode) ;
	return check;
}

/**
 * 设置内存节点的校验码以便查错
 */
static void setCheckSum(MemoryNodeHead *head)
{
	head->mCheckSum  = getCheckSum(head);
}

/**
 * 校验内存节点的校验码是否合法
 */
static int checkSum(MemoryNodeHead *head)
{
	return head->mCheckSum == getCheckSum(head);
}

/**
 * 分配内存
 * 在用完这段内存后记得调用free函数释放内存。
 * 记住，一定不要写越界，否则很容易导致内存分配链崩溃，然后系统跑飞。
 */
void* malloc(uint32_t size)
{
	MemoryNodeHead *proNode = mMainMemoryPage.mHead;
	MemoryNodeHead *nextNode = proNode->mNextNode;
	uint32_t needSize = size+sizeof(MemoryNodeHead);
	while(nextNode!=NULL)
	{
		// 计算两个节点间剩余的空间 
		uint32_t endptr = (((uint32_t)proNode)+sizeof(MemoryNodeHead)+(proNode->mLength));
		uint32_t space  = ((uint32_t)nextNode) - endptr;
		if(space<0)
		{// 如果走到了这里，那么恭喜你，内存分配链已经崩溃（四分五裂！）。重启系统吧！
			// TODO reboot system now.
			KLOGE("Memory link break!\n");
			return NULL;
		}
		
		if( space >= needSize )
		{
			if((!checkSum(proNode))||(!checkSum(nextNode)))
			{ // 内存节点头部校验失败！一定是某个地方写越界了！
				// TODO reboot system now.
				KLOGE("malloc:%p:%p:Memory head check failed!",proNode,nextNode);
				dumpMemoryNodeHead(proNode);
				dumpMemoryNodeHead(nextNode);
				KLOGD("used/totla:%d/%d",memoryUsed(),memoryTotal());
				return NULL;
			}
			MemoryNodeHead *now = (MemoryNodeHead *)endptr;
			now->mNextNode = nextNode;
			now->mProNode  = proNode;
			now->mLength   = size;
			setCheckSum(now);
			proNode->mNextNode = now;
			setCheckSum(proNode);
			nextNode->mProNode = now;
			setCheckSum(nextNode);
			return (void *)((uint32_t)now+sizeof(MemoryNodeHead));
		}
		proNode  = proNode->mNextNode;
		nextNode = proNode->mNextNode;
	}
	//没有更多的节点了,此时proNode为最后一个节点
	
	uint32_t endptr = (((uint32_t)proNode)+sizeof(MemoryNodeHead)+(proNode->mLength));
	uint32_t space  = ((uint32_t)mMainMemoryPage.mEndAddr) - endptr;
	
	if(space<0)
	{// 如果走到了这里，那么恭喜你，内存分配链已经崩溃（四分五裂！）。重启系统吧！
		// TODO reboot system now.
		KLOGE("malloc:Memory link break!\n");
		return NULL;
	}
	if( space >= needSize )
	{
		if((!checkSum(proNode)))
		{ // 内存节点头部校验失败！一定是某个地方写越界了！
			// TODO reboot system now.
			KLOGE("malloc:%p:Memory head check failed!",proNode);
			dumpMemoryNodeHead(proNode);
			KLOGD("used/totla:%d/%d",memoryUsed(),memoryTotal());
			return NULL;
		}
		MemoryNodeHead *now = (MemoryNodeHead *)endptr;
		now->mNextNode = NULL;
		now->mProNode  = proNode;
		now->mLength   = size;
		setCheckSum(now);
		proNode->mNextNode = now;
		setCheckSum(proNode);
		
		mMainMemoryPage.mMemoryUsed += needSize;
		
		return (void *)((uint32_t)now+sizeof(MemoryNodeHead));
	}
	KLOGE("malloc return NULL\n");
	return NULL;
}

void free(void *mem)
{
	if(mem==NULL)
	{
		KLOGE("free:NULL");
		return;
	}
	/**
	 * 得到内存节点头
	 */
	MemoryNodeHead *now = (MemoryNodeHead *)((uint32_t)mem - sizeof(MemoryNodeHead) );
	
	if((!checkSum(now)))
	{ // 内存节点头部校验失败！一定是某个地方写越界了！或者传了个假地址。
		// TODO reboot system now.
		KLOGE("free:%p:Memory head check failed!",now);
		return ;
	}
	
	if(now == mMainMemoryPage.mHead )
	{
		// 你TM逗我呢？你想把头都删掉？门都没有！
		return ;
	}
	
	MemoryNodeHead *proNode  = now->mProNode;  // 不可能为NULL  
	MemoryNodeHead *nextNode = now->mNextNode; // 可能为NULL 
	
	proNode->mNextNode = nextNode;
	setCheckSum(proNode);
	if(nextNode!=NULL)
	{
		nextNode->mProNode = proNode;
		setCheckSum(nextNode);
	}
	// 重置校验部分。
	now->mCheckSum = 0;
	
	mMainMemoryPage.mMemoryUsed -= now->mLength + sizeof(MemoryNodeHead);
}

uint32_t memoryTotal()
{
	return mMainMemoryPage.mLength;
}

uint32_t memoryUsed()
{
	return mMainMemoryPage.mMemoryUsed;
}

uint32_t memoryFree()
{
	return memoryTotal()-memoryUsed();
}

static void testMemory()
{
	KLOGD("mMainMemoryPage:%p",&mMainMemoryPage);
	KLOGD("before used/totla:%d/%d",memoryUsed(),memoryTotal());
	int size = 4096;
	int **array=(int**)malloc(sizeof(int*)*size);
	int i=0;
	for(i=0;i<size;i++)
	{
		array[i]=(int*)malloc(sizeof(int)*10);
		
		int j;
		for(j=0;j<10;j++)
		{
			array[i][j]=(j<<24)|(j<<16)|(j<<8)|j;
		}
	}
	KLOGD("after  used/totla:%d/%d",memoryUsed(),memoryTotal());
	for(i=0;i<size;i++)
	{
		free(array[i]);
	}
	free(array);
	KLOGD("final  used/totla:%d/%d",memoryUsed(),memoryTotal());
}
