/*
  Copyright (c), 2022-2024, Oritek Tech. Co., Ltd.
 */

#ifndef PARTITION_COMMON_H
#define PARTITION_COMMON_H

#include <stdint.h>



#define PARTITION_NAME_SZ           64UL
#define FS_TYPE_SZ                  16UL
#define FLASH_TYPE_SZ               16UL
#define PARTITION_MAX_SIZE          (sizeof(partition_info_t)*128)

enum {
    OT_PARTITION_BLOCK = 1,
};

enum{
    PT_FAILED = -1,
    PT_SUCCEESS = 0,
};


#pragma pack(1)
typedef struct parttion_info{
    char partition_name[PARTITION_NAME_SZ];
    uint64_t start_addr;
    uint64_t length;
    char fs_type[FS_TYPE_SZ];
    char flash_type[FLASH_TYPE_SZ];
    uint32_t reserved[4];
}ot_partition_info_t;

typedef struct block_info{
    uint32_t block_type;
    uint32_t block_size;
    uint8_t block_payload[0];
}ot_partition_block_info_t;

typedef struct bmp_info{
    uint32_t bmp_info_size;
    uint32_t reserved[4];
    ot_partition_block_info_t blocks[0];
}ot_partition_bmp_info_t;

#pragma pack()


#endif /*PARTITION_COMMON_H*/