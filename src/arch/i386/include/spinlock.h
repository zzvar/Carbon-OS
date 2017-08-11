#ifndef __INCLUDE_SPINLOCK_H__
#define __INCLUDE_SPINLOCK_H__
#include <types.h>
typedef struct spinlock_t
{
    volatile uint32_t lock;
} spinlock_t;
#define SPIN_LOCK_UNLOCKED (spinlock_t) { 1 }
#define spin_lock_init(x) do { *(x) = SPIN_LOCK_UNLOCKED; } while (0)
static inline void spinlock_lock(spinlock_t *lock)
{
    __asm__ volatile ("\n1:\t"
                    "lock;decb %0\n\t"
                    "js 2f\n\t"
                    ".section .text.lock,\"ax\"\n"
                    "2:\t"
                    "cmpb $0,%0\n\t"
                    "rep;nop\n\t"
                    "jle 2b\n\t"
                    "jmp 1b\n\t"
                    ".previous"
                    : "=m" (lock -> lock) : : "memory");
}
static inline void spinlock_unlock(spinlock_t *lock)
{
    __asm__ volatile ("movb $1,%0"
                    : "=m" (lock -> lock) : : "memory");
}
#endif
