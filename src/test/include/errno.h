#ifndef __INCLUDE_ERRNO_H__
#define __INCLUDE_ERRNO_H__
#include <types.h>
#define E_UNSPECIFIED 1
#define E_BAD_PROC 2
#define E_INVAL 3
#define E_NO_MEM 4
#define E_NO_FREE_PROC 5
#define E_FAULT 6
#define MAXERROR 6
const char *strerr(int errno);
#endif
