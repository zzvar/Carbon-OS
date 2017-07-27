#ifndef __INCLUDE_ELF_H__
#define __INCLUDE_ELF_H__
#include <types.h>
#include <mboot.h>
#define ELF32_ST_TYPE(i) ((i) & 0xF)
typedef struct elf_section_header_t 
{
    uint32_t name;
    uint32_t type;
    uint32_t flags;
    uint32_t addr;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;
} __attribute__((packed)) elf_section_header_t;
typedef struct elf_symbol_t
{
    uint32_t name;
    uint32_t value;
    uint32_t size;
    uint32_t info;
    uint32_t other;
    uint32_t shndx;
} __attribute__((packed)) elf_symbol_t;
typedef struct elf_t
{
    elf_symbol_t *symtab;
    uint32_t symtabsz;
    const char *strtab;
    uint32_t strtabsz;
} elf_t;
#endif
