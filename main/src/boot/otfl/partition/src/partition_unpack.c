/* #include <stdint.h> */
/* #include <stddef.h> */
/* #include <string.h> */
/* #include <printf.h> */
#include "ot_type.h"
#include "ot_string.h"
#include "ot_partition_unpack.h"

//#define DEBUG
#ifdef DEBUG
#define pt_trace(format,arg...) printf("[trace] %s:%d" "  " format"",__func__, __LINE__,##arg)
#define pt_info(format,arg...) printf("[info] %s:%d" "  " format"",__func__, __LINE__,##arg)
#else
#define pt_trace(format,arg...)
#define pt_info(format,arg...)
#endif

#ifndef UINT32_MAX
#define UINT32_MAX      ((ot_u32)(-1))
#endif

#define uswap_16(x) \
	((((x) & 0xff00) >> 8) | \
	 (((x) & 0x00ff) << 8))
#define uswap_32(x) \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24))
#define _uswap_64(x, sfx) \
	((((x) & 0xff00000000000000##sfx) >> 56) | \
	 (((x) & 0x00ff000000000000##sfx) >> 40) | \
	 (((x) & 0x0000ff0000000000##sfx) >> 24) | \
	 (((x) & 0x000000ff00000000##sfx) >>  8) | \
	 (((x) & 0x00000000ff000000##sfx) <<  8) | \
	 (((x) & 0x0000000000ff0000##sfx) << 24) | \
	 (((x) & 0x000000000000ff00##sfx) << 40) | \
	 (((x) & 0x00000000000000ff##sfx) << 56))
#if defined(__GNUC__)
# define uswap_64(x) _uswap_64(x, ull)
#else
# define uswap_64(x) _uswap_64(x, )
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
# define cpu_to_le16(x)		(x)
# define cpu_to_le32(x)		(x)
# define cpu_to_le64(x)		(x)
# define le16_to_cpu(x)		(x)
# define le32_to_cpu(x)		(x)
# define le64_to_cpu(x)		(x)
# define cpu_to_be16(x)		uswap_16(x)
# define cpu_to_be32(x)		uswap_32(x)
# define cpu_to_be64(x)		uswap_64(x)
# define be16_to_cpu(x)		uswap_16(x)
# define be32_to_cpu(x)		uswap_32(x)
# define be64_to_cpu(x)		uswap_64(x)
#else
# define cpu_to_le16(x)		uswap_16(x)
# define cpu_to_le32(x)		uswap_32(x)
# define cpu_to_le64(x)		uswap_64(x)
# define le16_to_cpu(x)		uswap_16(x)
# define le32_to_cpu(x)		uswap_32(x)
# define le64_to_cpu(x)		uswap_64(x)
# define cpu_to_be16(x)		(x)
# define cpu_to_be32(x)		(x)
# define cpu_to_be64(x)		(x)
# define be16_to_cpu(x)		(x)
# define be32_to_cpu(x)		(x)
# define be64_to_cpu(x)		(x)
#endif


static ot_s32 do_unpack(ot_partition_bmp_info_t * bmp_info, ot_partition_info_t **part_info, ot_u32 part_index,
                        const ot_s8 *part_name, ot_partition_env_info_t **env_info)
{
    ot_s32 res = OT_FAILURE;
    if(bmp_info){
        ot_u32 offset = 0;
        ot_u32 bmp_sz = le32_to_cpu(bmp_info->bmp_info_size);
        ot_u32 block_sz = bmp_sz - sizeof(ot_partition_bmp_info_t);
        ot_u32 block_header_sz = sizeof(ot_partition_block_info_t);
        ot_partition_block_info_t *block = NULL;
        ot_partition_info_t *par = NULL;
        ot_partition_env_info_t *env = NULL;
        ot_u32 cur_part_idx = 0;
        ot_u32 part_sz = sizeof(ot_partition_info_t);
        pt_trace("bmp_sz:%d bmp_info:%p\n",bmp_sz, bmp_info);
        for (offset = 0;(offset < block_sz) && (offset < OT_PARTITION_MAX_SIZE); offset += block_header_sz){
            block = (ot_partition_block_info_t *)((ot_u8 *)&bmp_info->blocks[0] + offset);
            pt_trace("block:%p offset:%d block_type:%d\n", block, offset, block->block_type);
            if (le32_to_cpu(block->block_type) == OT_PARTITION_BLOCK){
                ot_u32 par_offset = 0;
                offset += block->block_size;
                if (part_info == NULL){
                    continue;
                }
                for (par_offset =0; par_offset < block->block_size; par_offset+= part_sz){
                    par = (ot_partition_info_t *)((ot_u8 * )block->block_payload + par_offset);
                    if ((part_index == cur_part_idx) || ((part_name != NULL) &&
                        (0 == ot_strcmp(part_name,par->partition_name)))){
                        *part_info = par;
                        res = OT_SUCCESS;
                        pt_trace("{\n");
                        pt_trace("\t name:%s\n", par->partition_name);
                        pt_trace("\t flash_type:%s\n", par->flash_type);
                        pt_trace("\t fs_type:%s\n", par->fs_type);
                        pt_trace("\t start_addr:%lx\n", le64_to_cpu(par->start_addr));
                        pt_trace("\t length:%lx\n", le64_to_cpu(par->length));
                        pt_trace("}\n");
                        goto out;
                    }
                    cur_part_idx++;
                }
            }

            if (le32_to_cpu(block->block_type) == OT_ENV_BLOCK){
                env = (ot_partition_env_info_t *)block->block_payload;
                offset += block->block_size;
                if (env_info != NULL){
                    *env_info = env;
                    res = OT_SUCCESS;
                    pt_trace("{\n");
                    pt_trace("\t env:%s\n", env->data);
                    pt_trace("}\n");
                    goto out;
                }else{
                    continue;
                }
            }

        }
    }
    out:
    return res;
}

ot_s32 ot_partition_get_partition_info_by_name(ot_partition_bmp_info_t *bmp_info, ot_partition_info_t **part_info, const ot_s8 *part_name)
{
    ot_s32 res = OT_FAILURE;
    if ((bmp_info != NULL) && (part_info != NULL)){
        return do_unpack(bmp_info, part_info, UINT32_MAX,part_name,  NULL);
    }

    return res;
}

ot_s32 ot_partition_get_partition_info_by_index(ot_partition_bmp_info_t *bmp_info, ot_partition_info_t **part_info, ot_u32 part_wanted_index)
{
    ot_s32 res = OT_FAILURE;
    if ((bmp_info != NULL) && (part_info != NULL)){
        return do_unpack(bmp_info, part_info,part_wanted_index,NULL,  NULL);
    }

    return res;
}

ot_s32 ot_partition_get_env_info(ot_partition_bmp_info_t * bmp_info, ot_partition_env_info_t **env_info)
{
    ot_s32 res = OT_FAILURE;
    if ((bmp_info != NULL) && (env_info != NULL)){
        return do_unpack(bmp_info, NULL, 0,NULL, env_info);
    }

    return res;
}
