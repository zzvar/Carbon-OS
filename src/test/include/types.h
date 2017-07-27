#ifndef __INCLUDE_TYPES_H__
#define __INCLUDE_TYPES_H__
#ifndef NULL
	#define NULL 0
#endif
#ifndef TRUE
	#define TRUE 1
	#define FALSE 0
#endif
#define __UNUSED__ __attribute__((unsed))
typedef enum bool
{
    false = 0,
    true = 1
} bool;
typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned int size_t;
typedef int ssize_t;
typedef uint32_t time_t;
typedef int32_t pid_t;
typedef uint32_t pgd_t;
typedef uint32_t pte_t;
typedef struct { volatile int counter; } atomic_t;
struct list_head { struct list_head *next,*prev; }
#endif
