/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef SCENE_SETPARAM_INNER_H
#define SCENE_SETPARAM_INNER_H

#include "ot_scene_setparam.h"


#ifdef __cplusplus
extern "C" {
#endif

#define PIC_WIDTH_1080P 1920
#define PIC_HEIGHT_1080P 1080
#define LCU_ALIGN_H265 64
#define MB_ALIGN_H264 16

#define scene_div_0to1(a) (((a) == 0) ? 1 : (a))

#define SCENE_SHADING_TRIGMODE_L2H 1

#define SCENE_SHADING_TRIGMODE_H2L 2



ot_scene_pipe_param *get_pipe_params(ot_void);
ot_bool *get_isp_pause(ot_void);

#define check_scene_return_if_pause(pipe_hdl) do {   \
    	ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };  \
    	ot_vi_pipe_id master_pipe_id = 0;                          \
    	ot_s32 pipe_num = 0;                                       \
    	ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num); \
    	master_pipe_id = pipe_id[0];                               \
        if((ret == OT_SUCCESS ) && \
           (get_isp_pause()[master_pipe_id]) == OT_TRUE) { \
            return OT_SUCCESS;               \
        }                                    \
    } while (0)

#define check_scene_ret(ret) do {                                                    \
        if ((ret) != OT_SUCCESS) {                                                   \
            printf("Failed at %s: LINE: %d with %#x!\n", __FUNCTION__, __LINE__, ret); \
        }                                                                            \
    } while (0)

ot_u32 scene_get_level_ltoh(ot_u64 value, ot_u32 count, const ot_u64 *thresh);
ot_u32 scene_get_level_ltoh_u32(ot_u32 value, ot_u32 count, const ot_u32 *thresh);
ot_u32 scene_interpulate(ot_u64 middle, ot_u64 left, ot_u64 left_value, ot_u64 right, ot_u64 right_value);
ot_u32 scene_time_filter(ot_u32 param0, ot_u32 param1, ot_u32 time_cnt, ot_u32 index);
//ot_void scene_set_static_h265_avbr(ot_venc_rc_param *rc_param, ot_u8 index);
//ot_void scene_set_static_h265_cvbr(ot_venc_rc_param *rc_param, ot_u8 index);
ot_void scene_set_isp_attr(ot_vi_pipe_handle pipe_hdl,set_isp_attr_param param, const ot_u32 *ratio_level_thresh,
    const ot_u32 *iso_level_thresh, ot_eis_vi_drc_attr *isp_drc_attr);

ot_s32 scene_blend_tone_mapping(ot_vi_pipe_handle pipe_hdl, ot_u8 index,
    ot_eis_scene_dynamic_drc_coef *drc_coef, ot_eis_vi_drc_attr *isp_drc_attr);
ot_s32 scene_set_tone_mapping_value(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u32 k,
    const ot_eis_vi_alg_state_info *inner_state_info, ot_eis_vi_drc_attr *isp_drc_attr);
ot_s32 scene_set_nr_attr_para(ot_u8 index, ot_u32 iso, ot_eis_vi_nr_attr *nr_attr, const ot_scene_nr_para *nr_para);
ot_s32 scene_set_nr_wdr_ratio_para(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u32 wdr_ratio, ot_u32 ratio_index,
    ot_eis_vi_nr_attr *nr_attr);
ot_s32 scene_set_3dnr(ot_eis_handle vi_pipe_hdl, ot_eis_handle vproc_hdl, const ot_scene_3dnr *_3dnr, ot_u8 index, ot_eis_3dnr_pos pos_3dnr);
ot_void scene_set_3dnr_nrx_nry(const _3dnr_nrx_pack *pack);
ot_void scene_set_3dnr_nrx_iey(const _3dnr_nrx_pack *pack);
ot_void scene_set_3dnr_nrx_sfy(const _3dnr_nrx_pack *pack);
ot_void scene_set_3dnr_nrx_tfy(const _3dnr_nrx_pack *pack, ot_eis_3dnr_pos pos_3dnr);
ot_void scene_set_3dnr_nrx_mdy(const _3dnr_nrx_pack *pack);
ot_void scene_set_3dnr_nrx_nrc0(const _3dnr_nrx_pack *pack);
ot_void scene_set_3dnr_nrx_nrc1(const _3dnr_nrx_pack *pack);
ot_void scene_set_3dnr_nrx_adv_sfy(const _3dnr_nrx_pack *pack);
ot_void scene_set_3dnr_nrx_adv_iey(const _3dnr_nrx_pack *pack);

//ot_s32 scene_set_qp(ot_u32 pic_width, ot_u32 pic_height, ot_u32 max_bitrate, ot_payload_type type,
//    ot_venc_rc_param *rc_param);
//ot_s32 get_iso_mean_qp_chn_attr(ot_vi_pipe_id vi_pipe, ot_u32 *iso, ot_u32 *mean_qp, ot_venc_chn_attr *venc_chn_attr);
ot_void calculate_manual_percent_index(ot_u8 pipe_param_index, ot_u32 sum_iso, ot_u32 *out_index);
//ot_void set_initial_percent(ot_payload_type type, ot_u32 index, ot_venc_rc_param *rc_param,
//    const ot_scene_pipe_param *param, ot_s32 *percent);
//ot_void set_min_qp_delta_when_iso_larger(ot_payload_type type, ot_u32 sum_mean_qp, ot_s32 percent,
//    ot_venc_rc_param *rc_param);
//ot_void set_min_qp_delta_when_iso_less(ot_payload_type type, ot_u32 sum_mean_qp, ot_venc_rc_param *rc_param,
//    ot_venc_chn_attr *venc_chn_attr);


#ifdef __cplusplus
}
#endif

#endif
