#ifndef __INCLUDE_COMMON_H__
#define __INCLUDE_COMMON_H__
#include "types.h"
#define FL_CF 0x00000001
#define FL_PF 0x00000004
#define FL_AF 0x00000010
#define FL_ZF 0x00000040
#define FL_SF 0x00000080
#define FL_TF 0x00000100
#define FL_IF 0x00000200
#define FL_DF 0x00000400
#define FL_OF 0x00000800
#define FL_IOPL_MASK 0x00003000
#define FL_IOPL_0 0x00000000
#define FL_IOPL_1 0x00001000
#define FL_IOPL_2 0x00002000
#define FL_IOPL_3 0x00003000
#define FL_NT 0x00004000
#define FL_RF 0x00010000
#define FL_VM 0x00020000
#define FL_AC 0x00040000
#define FL_VIF 0x00080000
#define FL_VIP 0x00100000
#define FL_ID 0x00200000
#define __barrier__() __asm__ volatile ("" ::: "memory")
static inline void outb(uint16_t port,uint8_t value)
{
    __asm__ volatile ("outb %1,%0" : : "dN" (port),"a" (value));
}
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ("inb %1,%0" : "=a" (ret) : "dN" (port));
    return ret;
}
static inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    __asm__ volatile ("inw %1,%0" : "=a" (ret) : "dN" (port));
    return ret;
}
static inline void insl(uint32_t port,void *addr,int cnt)
{
    __asm__ volatile (
                    "cld;"
                    "repne;insl;"
                    : "=D" (addr),"=c" (cnt)
                    : "d" (port),"0" (addr),"1" (cnt)
                    : "memory","cc");
}
static inline void outsl(uint32_t port,const void *addr,int cnt)
{
    __asm__ volatile (
                    "cld;"
                    "repne;outsl;"
                    : "=S" (addr),"=c" (cnt)
                    : "d" (port),"0" (addr),"1" (cnt)
                    : "memory","cc");
}
static inline void enable_intr(void) { __asm__ volatile ("sti"); }
static inline void disable_intr(void) { __asm__ volatile ("cli" ::: "memory"); }
static inline void cpu_hlt(void) { __asm__ volatile ("hlt"); }
static inline uint32_t read_eflags(void)
{
    uint32_t eflags;
    __asm__ volatile ("pushfl;popl %0" : "=r" (eflags));
    return eflags;
}
static inline void write_eflags(uint32_t eflags) { __asm__ volatile ("pushl %0;popfl" :: "r" (eflags)); }
static inline void switch_pgd(uint32_t pd) { __asm__ volatile ("mov %0,%%cr3" : : "r" (pd)); }
static inline void tlb_reload_page(uint32_t va) { __asm__ volatile ("invlpg (%0)" : : "a" (va)); }
static inline void load_esp(uint32_t esp) { __asm__ volatile ("mov %0,%%esp" : : "r" (esp)); }
#endif
