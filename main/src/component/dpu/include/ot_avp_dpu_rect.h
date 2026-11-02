/******************************************************************************
 Copyright (C), 2024 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : the dpu rect api
Created       : 2024/09/23
Last Modified :
******************************************************************************/

#ifndef OT_AVP_DPU_RECT_H
#define OT_AVP_DPU_RECT_H

#include "ot_avp_dpu_rect_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*==================================================================
 * Name：ot_avp_dpu_rect_load_lut
 * Author：
 * Date：2024-09-25
 * Function：dpu rect load lut
 * Input： ot_avp_dpu_rect_mem_info lut_mem
 * Output: ot_u32 rect_lut_id
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_load_lut(const ot_avp_dpu_rect_mem_info *lut_mem, ot_s32 *rect_lut_id);

/*==================================================================
 * Name：ot_avp_dpu_rect_unload_lut
 * Author：
 * Date：2024-09-25
 * Function：dpu rect unload lut
 * Input：  ot_u32 rect_lut_id
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_unload_lut(ot_s32 rect_lut_id);

/*==================================================================
 * Name：ot_avp_dpu_rect_update_lut
 * Author：
 * Date：2024-09-25
 * Function：dpu rect unload lut
 * Input：  ot_avp_dpu_rect_mem_info lut_mem[]
            ot_u32 rect_lut_id[]
            ot_eis_handle grp_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_update_lut(const ot_avp_dpu_rect_mem_info lut_mem[OT_AVP_DPU_RECT_MAX_PIPE_NUM],
    const ot_s32 rect_lut_id[OT_AVP_DPU_RECT_MAX_PIPE_NUM], ot_eis_handle grp_hdl);

/*==================================================================
 * Name：ot_avp_dpu_rect_create_grp
 * Author：
 * Date：2024-09-25
 * Function：dpu rect create group
 * Input：  ot_avp_dpu_rect_grp_attr grp_attr
 * Output:  ot_eis_handle grp_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_create_grp(ot_eis_handle* grp_hdl, const ot_avp_dpu_rect_grp_attr *grp_attr);

/*==================================================================
 * Name：ot_avp_dpu_rect_destroy_grp
 * Author：
 * Date：2024-09-25
 * Function：dpu rect destroy group
 * Input：  ot_eis_handle grp_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_destroy_grp(ot_eis_handle grp_hdl);

/*==================================================================
 * Name：ot_avp_dpu_rect_set_grp_attr
 * Author：
 * Date：2024-09-25
 * Function：dpu rect set group attr
 * Input：  ot_eis_handle grp_hdl
 *          ot_avp_dpu_rect_grp_attr grp_attr
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_set_grp_attr(ot_eis_handle grp_hdl, const ot_avp_dpu_rect_grp_attr *grp_attr);

/*==================================================================
 * Name：ot_avp_dpu_rect_get_grp_attr
 * Author：
 * Date：2024-09-25
 * Function：dpu rect get group attr
 * Input：  ot_eis_handle grp_hdl
 * Output:  ot_avp_dpu_rect_grp_attr grp_attr
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_get_grp_attr(ot_eis_handle grp_hdl, ot_avp_dpu_rect_grp_attr *grp_attr);

/*==================================================================
 * Name：ot_avp_dpu_rect_create_chn
 * Author：
 * Date：2024-09-25
 * Function：dpu rect create chn
 * Input：  ot_eis_handle grp_hdl
 * Output:  ot_eis_handle chn_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_create_chn(ot_eis_handle grp_hdl, ot_eis_handle* chn_hdl);

/*==================================================================
 * Name：ot_avp_dpu_rect_destroy_chn
 * Author：
 * Date：2024-09-25
 * Function：dpu rect destroy chn
 * Input：  ot_eis_handle chn_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_destroy_chn(ot_eis_handle chn_hdl);

/*==================================================================
 * Name：ot_avp_dpu_rect_set_chn_attr
 * Author：
 * Date：2024-09-25
 * Function：dpu rect set chn attr
 * Input：  ot_eis_handle chn_hdl
 *          ot_avp_dpu_rect_chn_attr chn_attr
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_set_chn_attr(ot_eis_handle chn_hdl, const ot_avp_dpu_rect_chn_attr *chn_attr);

/*==================================================================
 * Name：ot_avp_dpu_rect_get_chn_attr
 * Author：
 * Date：2024-09-25
 * Function：dpu rect get chn attr
 * Input：  ot_eis_handle chn_hdl
 * Output:  ot_avp_dpu_rect_chn_attr chn_attr
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_get_chn_attr(ot_eis_handle chn_hdl, ot_avp_dpu_rect_chn_attr *chn_attr);

/*==================================================================
 * Name：ot_avp_dpu_rect_enable_chn
 * Author：
 * Date：2024-09-25
 * Function：dpu rect enable chn
 * Input：  ot_eis_handle chn_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_enable_chn(ot_eis_handle chn_hdl);

/*==================================================================
 * Name：ot_avp_dpu_rect_disable_chn
 * Author：
 * Date：2024-09-25
 * Function：dpu rect disable chn
 * Input：  ot_eis_handle chn_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_disable_chn(ot_eis_handle chn_hdl);

/*==================================================================
 * Name：ot_avp_dpu_rect_start_grp
 * Author：
 * Date：2024-09-25
 * Function：dpu rect start group
 * Input：  ot_eis_handle grp_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_start_grp(ot_eis_handle grp_hdl);

/*==================================================================
 * Name：ot_avp_dpu_rect_stop_grp
 * Author：
 * Date：2024-09-25
 * Function：dpu rect stop group
 * Input：  ot_eis_handle grp_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_stop_grp(ot_eis_handle grp_hdl);

/*==================================================================
 * Name：ot_avp_dpu_rect_get_frame
 * Author：
 * Date：2024-09-25
 * Function：dpu rect get frame
 * Input：  ot_eis_handle grp_hdl
 *          ot_s32 milli_sec
 * Output:  ot_avp_dpu_rect_frame_info rect_frame_info
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_get_frame(ot_eis_handle grp_hdl, ot_s32 milli_sec, ot_avp_dpu_rect_frame_info *rect_frame_info);

/*==================================================================
 * Name：ot_avp_dpu_rect_release_frame
 * Author：
 * Date：2024-09-25
 * Function：dpu rect release frame
 * Input：  ot_eis_handle grp_hdl
 *          ot_avp_dpu_rect_frame_info rect_frame_info
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_release_frame(ot_eis_handle grp_hdl, const ot_avp_dpu_rect_frame_info *rect_frame_info);

/*==================================================================
 * Name：ot_avp_dpu_rect_send_frame
 * Author：
 * Date：2024-09-25
 * Function：dpu rect send frame
 * Input：  ot_eis_handle grp_hdl
 *          ot_eis_img_frame left_frame
 *          ot_eis_img_frame right_frame
 *          ot_s32 milli_sec
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_avp_dpu_rect_send_frame(ot_eis_handle grp_hdl,
    const ot_eis_img_frame *left_frame, const ot_eis_img_frame *right_frame, ot_s32 milli_sec);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* OT_AVP_DPU_RECT_H */
