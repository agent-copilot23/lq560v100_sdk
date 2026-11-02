
/***********************************************************************************
*  Copyright (C), 2023 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
*  ********************************************************************************
*  Description   : buffer_pool common description
*  Created       : 2023/09/10
*  Last Modified : 2023/09/10
*  Group         : SDK Team
*  ********************************************************************************/

#ifndef OT_BUFFER_POOL_COMMON_H
#define OT_BUFFER_POOL_COMMON_H

#include "ot_eis_type.h"
#include "ot_eis_errno.h"
#include "ot_eis_mod.h"
#include "ot_smr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OT_BUFFER_POOL_INVALID_POOL_HDL (0xFFFFFFFF)

#define OT_MEMORY_BLOCK_SET_MAX   (16)

typedef struct {
    ot_s8  region_name[OT_SMR_REGION_NAME_LEN_MAX];
    ot_u32 cnt;
    ot_u32 size;
} ot_video_buffer_blk;

typedef struct {
    ot_video_buffer_blk buf_blks[OT_MEMORY_BLOCK_SET_MAX];
    ot_u32 cnt;//set个数，block_set_pool（一组size组成一组set）
} ot_video_buffer_attr;

typedef struct {
    ot_phys_addr phys_addr;
    ot_u32 block_size;//block 长度
} ot_buffer_pool_blk;

typedef enum {
    OT_BUFPOOL_NOT_INIT = 0x20,
    OT_BUFPOOL_NOT_READY,
    OT_BUFPOOL_BUSY,
    OT_BUFPOOL_NO_BUF,
    OT_BUFPOOL_ALREADY_INIT,
    OT_BUFPOOL_BUTT
} ot_buf_pool_err_code;

#define OT_ERR_BUFPOOL_DEFINE(errno)      OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_BUFPOOL, errno)

#define OT_ERR_BUFPOOL_NULL_PTR           OT_ERR_BUFPOOL_DEFINE(OT_EIS_ERRNO_NULL_PTR)
#define OT_ERR_BUFPOOL_ILLEGAL_PAARAM     OT_ERR_BUFPOOL_DEFINE(OT_EIS_ERRNO_ILLEGAL_PARAM)
#define OT_ERR_BUFPOOL_NO_MEM             OT_ERR_BUFPOOL_DEFINE(OT_EIS_ERRNO_NO_MEM)
#define OT_ERR_BUFPOOL_NO_BUF             OT_ERR_BUFPOOL_DEFINE(OT_BUFPOOL_NO_BUF)
#define OT_ERR_BUFPOOL_NOT_INIT           OT_ERR_BUFPOOL_DEFINE(OT_BUFPOOL_NOT_INIT)
#define OT_ERR_BUFPOOL_ALREADY_INIT       OT_ERR_BUFPOOL_DEFINE(OT_BUFPOOL_ALREADY_INIT)
#define OT_ERR_BUFPOOL_NOT_READY          OT_ERR_BUFPOOL_DEFINE(OT_BUFPOOL_NOT_READY)
#define OT_ERR_BUFPOOL_NOT_SUPPORT        OT_ERR_BUFPOOL_DEFINE(OT_EIS_ERRNO_NO_SUPPORT)
#define OT_ERR_BUFPOOL_BUSY               OT_ERR_BUFPOOL_DEFINE(OT_BUFPOOL_BUSY)
#define OT_ERR_BUFPOOL_UNEXIST            OT_ERR_BUFPOOL_DEFINE(OT_EIS_ERRNO_NO_EXIST)
#define OT_ERR_BUFPOOL_SIZE_NOT_ENOUGH    OT_ERR_BUFPOOL_DEFINE(OT_EIS_ERRNO_OVER_SIZE)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OT_BUFFER_POOL_COMMON_H */
