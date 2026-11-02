#ifndef __PARTITION_COMMON_H__
#define __PARTITION_COMMON_H__


#include "ot_type.h"


#define OT_PARTITION_NAME_SZ           64UL
#define OT_FS_TYPE_SZ                  16UL
#define OT_FLASH_TYPE_SZ               16UL
#define OT_ENV_INFO_SIZE               (4UL +2048UL)
#define OT_PARTITION_MAX_SIZE          (OT_ENV_INFO_SIZE + sizeof(ot_partition_info_t)*128)

enum {
    OT_PARTITION_BLOCK = 1,
    OT_ENV_BLOCK,
};

#pragma pack(1)
typedef struct parttion_info{
    ot_s8 partition_name[OT_PARTITION_NAME_SZ];
    ot_u64 start_addr;
    ot_u64 length;
    ot_s8 fs_type[OT_FS_TYPE_SZ];
    ot_s8 flash_type[OT_FLASH_TYPE_SZ];
    ot_s32 reserved[4];
}ot_partition_info_t;


typedef struct env_info{
	ot_s32	crc;		/* CRC32 over data bytes	*/
	ot_u8	data[OT_ENV_INFO_SIZE - sizeof(ot_s32)]; /* Environment data		*/
}ot_partition_env_info_t;


typedef struct block_info{
    ot_s32 block_type;
    ot_s32 block_size;
    ot_u8 block_payload[0];
}ot_partition_block_info_t;

typedef struct bmp_info{
    ot_s32 bmp_info_size;
    ot_s32 reserved[4];
    ot_partition_block_info_t blocks[0];
}ot_partition_bmp_info_t;
#pragma pack()


#endif /*__PARTITION_COMMON_H__*/
