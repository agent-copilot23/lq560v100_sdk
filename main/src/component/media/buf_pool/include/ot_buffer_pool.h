/***********************************************************************************
*  Copyright (C), 2023 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
*  ********************************************************************************
*  Description   : buffer pool api
*  Created       : 2023/06/07
*  Last Modified : 2023/06/07
*  Group         : SDK Team
*  ********************************************************************************/
#ifndef BUFFER_POOL_API_H
#define BUFFER_POOL_API_H

#include "ot_eis_type.h"
#include "ot_buffer_pool_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================
 * Name: ot_buffer_pool_init
 * Author：
 * Date：2023-07-04
 * Function：buffer pool init
 * Input： none
 * Output: none
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_buffer_pool_init(ot_void);

/*==================================================================
 * Name: ot_buffer_pool_deinit
 * Author：
 * Date：2023-07-04
 * Function：buffer pool deinit
 * Input： none
 * Output: none
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_buffer_pool_deinit(ot_void);

/*==================================================================
 * Name: ot_buffer_pool_create
 * Author：
 * Date：2023-07-04
 * Function：buffer pool create
 * Input： ot_video_buffer_attr* attr
 * Output: ot_eis_handle pool_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：当前一个pool只支持一个size，即attr->cnt 当前只能设置为1
==================================================================*/
ot_s32 ot_buffer_pool_create(ot_eis_handle* pool_hdl, const ot_video_buffer_attr* attr);

/*==================================================================
 * Name: ot_buffer_pool_destroy
 * Author：
 * Date：2023-07-04
 * Function：buffer pool destroy
 * Input： ot_eis_handle pool_hdl
 * Output: none
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_buffer_pool_destroy(ot_eis_handle pool_hdl);

/*==================================================================
 * Name: ot_buffer_pool_blk_acquire
 * Author：
 * Date：2023-07-04
 * Function：buffer pool acquire blk
 * Input： ot_eis_handle pool_hdl
 *         ot_u32 req_size
 * Output: ot_buffer_pool_blk* blk
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_buffer_pool_blk_acquire(ot_eis_handle pool_hdl, ot_u32 req_size, ot_buffer_pool_blk* blk);

/*==================================================================
 * Name: ot_buffer_pool_blk_release
 * Author：
 * Date：2023-07-04
 * Function：buffer pool release blk
 * Input： ot_eis_handle pool_hdl
 *         ot_buffer_pool_blk blk
 * Output: none
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_buffer_pool_blk_release(ot_eis_handle pool_hdl, const ot_buffer_pool_blk* blk);

/*==================================================================
 * Name: ot_buffer_pool_blk_2_pool_hdl
 * Author：
 * Date：2023-07-04
 * Function：buffer pool blk to pool
 * Input： ot_buffer_pool_blk blk
 * Output: ot_eis_handle pool_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_buffer_pool_blk_2_pool_hdl(ot_eis_handle* pool_hdl, const ot_buffer_pool_blk* blk);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OT_BUFFER_POOL_API_H */