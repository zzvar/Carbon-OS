#ifndef __INCLUDE_VARGS_H__
#define __INCLUDE_VARGS_H__
typedef __builtin_va_list va_list;
#define va_start(ap,start) (__builtin_va_start(ap,list))
#define va_arg(ap,type) (__builtin_va_arg(ap,type))
#define va_end(ap)
#endif
