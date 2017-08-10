#ifndef __INCLUDE_MBR_H__
#define __INCLUDE_MBR_H__
#include <types.h>
#include <block_dev.h>
#define SECTION_SIZE 512
#define MBR_CODE_LENGTH 446
#define PARTITION_SIZE 16
#define PARTITION_COUNT 4
typedef struct partition_info_t
{
    uint8_t active_flag;
    uint8_t start_chs[3];
    uint8_t partition_type;
    uint8_t end_chs[3];
    uint32_t start_sector;
    uint32_t nr_sectors;
} __attribute__((packed)) partition_info_t;
typedef struct mbr_info_t
{
    uint8_t mbr_code[MBR_CODE_LENGTH];
    partition_info_t part[PARTITION_COUNT];
    uint16_t magic_num;
} __attribute__((packed)) mbr_info_t;
extern mbr_info_r mbr_info;
int read_mbr_info(block_dev_t *bdev);
void show_partition_info(void);
#endif
