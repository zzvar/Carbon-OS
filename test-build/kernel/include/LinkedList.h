/**
 * create on 2017/4/5
 * by wangchen11
 * [Carbon_OS team]
 */
#include <types.h>
typedef struct LinkedListNode_St LinkedListNode; 
typedef struct LinkedList_St LinkedList;
typedef int(*compare_fp)(const void *obj1,const void *obj2);

LinkedList* newLinkedList();
LinkedList* LinkedList_clone(LinkedList* thiz,LinkedList* other);
void deleteLinkedList(LinkedList* thiz);
BOOL LinkedList_isEmpty(LinkedList* thiz);
BOOL LinkedList_size(LinkedList* thiz);
void LinkedList_clean(LinkedList* thiz);
BOOL LinkedList_addAll(LinkedList* thiz,LinkedList* other);
BOOL LinkedList_addAt(LinkedList* thiz,const void *data,int pos);
BOOL LinkedList_addLast (LinkedList* thiz,const void *data);
BOOL LinkedList_addFirst(LinkedList* thiz,const void *data);
BOOL LinkedList_remove(LinkedList* thiz,const void *data,compare_fp cmp);
void *LinkedList_removeAt(LinkedList* thiz,int pos);
void *LinkedList_get(LinkedList* thiz,int pos);
BOOL LinkedList_push(LinkedList* thiz,const void *data);
void *LinkedList_pop(LinkedList* thiz);
void *LinkedList_peek(LinkedList* thiz);
BOOL LinkedList_enqueue(LinkedList* thiz,const void *data);
void *LinkedList_dequeue(LinkedList* thiz);
int  LinkedList_indexOf(LinkedList* thiz,const void *data,compare_fp cmp);
void LinkedList_sort(LinkedList* thiz,compare_fp cmp);

void testLinkedList();
