#ifndef __PARTITION_UNPACK_H__
#define __PARTITION_UNPACK_H__
#include "ot_partition_common.h"
#include "ot_partition_errno.h"

ot_s32 ot_partition_get_partition_info_by_name(ot_partition_bmp_info_t *bmp_info, ot_partition_info_t **part_info, const ot_s8 *part_name);
ot_s32 ot_partition_get_partition_info_by_index(ot_partition_bmp_info_t *bmp_info, ot_partition_info_t **part_info, ot_u32 part_wanted_index);
ot_s32 ot_partition_get_env_info(ot_partition_bmp_info_t * bmp_info, ot_partition_env_info_t **env_info);

#endif /*__PARTITION_UNPACK_H__*/
