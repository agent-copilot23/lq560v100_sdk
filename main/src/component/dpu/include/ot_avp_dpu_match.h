/******************************************************************************
 Copyright (C), 2024 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : the dpu match api
Created       : 2024/09/23
Last Modified :
******************************************************************************/

#ifndef OT_AVP_DPU_MATCH_H
#define OT_AVP_DPU_MATCH_H

#include "ot_avp_dpu_match_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*==================================================================
 * Name：ot_avp_dpu_match_get_assist_buf_size
 * Author：
 * Date：2024-09-25
 * Function：dpu match get assist buf size
 * Input： ot_u16 disp_num
 *         ot_u32 dst_height
 * Output: ot_u32 size
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_get_assist_buf_size(ot_u16 disp_num, ot_u32 dst_height, ot_u32 *size);

/*==================================================================
 * Name：ot_avp_dpu_match_create_grp
 * Author：
 * Date：2024-09-25
 * Function：dpu match create group
 * Input： ot_avp_dpu_match_grp_attr grp_attr
 * Output: ot_eis_handle grp_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_create_grp(ot_eis_handle *grp_hdl, const ot_avp_dpu_match_grp_attr *grp_attr);

/*==================================================================
 * Name：ot_avp_dpu_match_destroy_grp
 * Author：
 * Date：2024-09-25
 * Function：dpu match destroy group
 * Input：  ot_eis_handle grp_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_destroy_grp(ot_eis_handle grp_hdl);

/*==================================================================
 * Name：ot_avp_dpu_match_set_grp_attr
 * Author：
 * Date：2024-09-25
 * Function：dpu match set group attr
 * Input：  ot_eis_handle grp_hdl
 *          ot_avp_dpu_match_grp_attr grp_attr
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_set_grp_attr(ot_eis_handle grp_hdl, const ot_avp_dpu_match_grp_attr *grp_attr);

/*==================================================================
 * Name：ot_avp_dpu_match_get_grp_attr
 * Author：
 * Date：2024-09-25
 * Function：dpu match get group attr
 * Input：  ot_eis_handle grp_hdl
 * Output:  ot_avp_dpu_match_grp_attr grp_attr
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_get_grp_attr(ot_eis_handle grp_hdl, ot_avp_dpu_match_grp_attr *grp_attr);

/*==================================================================
 * Name：ot_avp_dpu_match_start_grp
 * Author：
 * Date：2024-09-25
 * Function：dpu match start group
 * Input：  ot_eis_handle grp_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_start_grp(ot_eis_handle grp_hdl);

/*==================================================================
 * Name：ot_avp_dpu_match_stop_grp
 * Author：
 * Date：2024-09-25
 * Function：dpu match stop group
 * Input：  ot_eis_handle grp_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_stop_grp(ot_eis_handle grp_hdl);

/*==================================================================
 * Name：ot_avp_dpu_match_create_chn
 * Author：
 * Date：2024-09-25
 * Function：dpu match create chn
 * Input：  ot_eis_handle grp_hdl
 * Output:  ot_eis_handle chn_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_create_chn(ot_eis_handle grp_hdl, ot_eis_handle* chn_hdl);

/*==================================================================
 * Name：ot_avp_dpu_match_destroy_chn
 * Author：
 * Date：2024-09-25
 * Function：dpu match destroy chn
 * Input：  ot_eis_handle chn_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_destroy_chn(ot_eis_handle chn_hdl);

/*==================================================================
 * Name：ot_avp_dpu_match_set_chn_attr
 * Author：
 * Date：2024-09-25
 * Function：dpu match set chn attr
 * Input：  ot_eis_handle chn_hdl
 *          ot_avp_dpu_match_chn_attr chn_attr
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_set_chn_attr(ot_eis_handle chn_hdl, const ot_avp_dpu_match_chn_attr *chn_attr);

/*==================================================================
 * Name：ot_avp_dpu_match_get_chn_attr
 * Author：
 * Date：2024-09-25
 * Function：dpu match get chn attr
 * Input：  ot_eis_handle chn_hdl
 * Output:  ot_avp_dpu_match_chn_attr chn_attr
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_get_chn_attr(ot_eis_handle chn_hdl, ot_avp_dpu_match_chn_attr *chn_attr);

/*==================================================================
 * Name：ot_avp_dpu_match_enable_chn
 * Author：
 * Date：2024-09-25
 * Function：dpu match enable chn
 * Input：  ot_eis_handle chn_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_enable_chn(ot_eis_handle chn_hdl);

/*==================================================================
 * Name：ot_avp_dpu_match_disable_chn
 * Author：
 * Date：2024-09-25
 * Function：dpu match disable chn
 * Input：  ot_eis_handle chn_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_disable_chn(ot_eis_handle chn_hdl);

/*==================================================================
 * Name：ot_avp_dpu_match_get_frame
 * Author：
 * Date：2024-09-25
 * Function：dpu match get frame
 * Input：  ot_eis_handle grp_hdl
 *          ot_s32 milli_sec
 * Output:  ot_avp_dpu_match_src_frame_info src_frame_info
 *          ot_avp_dpu_match_dst_frame_info dst_frame_info
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_get_frame(ot_eis_handle grp_hdl, ot_s32 milli_sec,
    ot_avp_dpu_match_src_frame_info *src_frame_info, ot_avp_dpu_match_dst_frame_info *dst_frame_info);

/*==================================================================
 * Name：ot_avp_dpu_match_release_frame
 * Author：
 * Date：2024-09-25
 * Function：dpu match release frame
 * Input：  ot_eis_handle grp_hdl
 *          ot_avp_dpu_match_src_frame_info src_frame_info
 *          ot_avp_dpu_match_dst_frame_info dst_frame_info
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_release_frame(ot_eis_handle grp_hdl,
    const ot_avp_dpu_match_src_frame_info *src_frame_info, const ot_avp_dpu_match_dst_frame_info *dst_frame_info);

/*==================================================================
 * Name：ot_avp_dpu_match_send_frame
 * Author：
 * Date：2024-09-25
 * Function：dpu match release frame
 * Input：  ot_eis_handle grp_hdl
 *          ot_avp_dpu_match_src_frame_info src_frame_info
 *          ot_s32 milli_sec
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_match_send_frame(ot_eis_handle grp_hdl,
    const ot_avp_dpu_match_src_frame_info *src_frame_info, ot_s32 milli_sec);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* OT_AVP_DPU_MATCH_H */

