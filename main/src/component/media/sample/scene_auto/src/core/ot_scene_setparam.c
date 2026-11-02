/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "securec.h"
#include "ot_eis_vi_api.h"
#include "ot_eis_vi_awb_api.h"
#include "ot_eis_vi_ae_api.h"
#include "ot_eis_vi_alg_api.h"
#include "ot_eis_venc.h"
#include "sample_comm.h"
#include "ot_scenecomm.h"
#include "scene_setparam_inner.h"
#include "ot_scene_setparam.h"
#include "ot_scene.h"
#include "ot_eis_math.h"
#include "ot_eis_scene_auto_api.h"


#ifdef __cplusplus
extern "C" {
#endif

#define SET_3DNR

ot_char g_dir_name[DIR_NAME_BUFF];
ot_u8 g_dynamic_vproc_times = 0;
ot_u16 g_fps_set_target[OT_SCENE_PIPE_MAX_NUM] = {0};

ot_void set_dir_name(const ot_char *dir_name)
{
    snprintf_truncated_s(g_dir_name, DIR_NAME_BUFF, "%s", dir_name);
}

static sample_vi_fpn_correction_cfg g_correction_cfg = {
    .op_mode       = OT_EIS_OP_MODE_MANUAL,
    .aibnr_mode    = OT_TRUE,
    .fpn_type      = OT_EIS_VI_FPN_TYPE_FRAME,
    .strength      = FPN_CORRECTION_STR,
    .pixel_format  = OT_EIS_IMAGE_FORMAT_BAYER_16BPP,
};

ot_s32 ot_scene_set_ae_weight_table(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    ot_s32 i, j;
    ot_s32 ret;
    ot_eis_vi_stats_cfg isp_stats_cfg;

    ret = ot_eis_vi_stats_get_cfg(pipe_hdl, &isp_stats_cfg);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    for (i = 0; i < OT_EIS_VI_AE_ZONE_ROW; i++) {
        for (j = 0; j < OT_EIS_VI_AE_ZONE_COLUMN; j++) {
            isp_stats_cfg.ae_cfg.weight[i][j] = get_pipe_params()[index].static_statistics.ae_weight[i][j];
        }
    }
    ret = ot_eis_vi_stats_set_cfg(pipe_hdl, &isp_stats_cfg);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_ae(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_ae != OT_TRUE) {
        return OT_SUCCESS;
    }
    ot_s32 ret;
    ot_eis_vi_exposure_attr exposure_attr;
    ot_eis_vi_ae_route_ex ae_route_ex;
    ot_eis_vi_public_attr pub_attr;
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_eis_vi_public_get_attr(pipe_hdl, &pub_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_ae_get_route_ex_attr(pipe_hdl, &ae_route_ex);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ae_route_ex.total_num = get_pipe_params()[index].static_ae_route_ex.total_num;
    for (ot_u32 i = 0; i < ae_route_ex.total_num; i++) {
        ae_route_ex.route_ex_node[i].int_time = get_pipe_params()[index].static_ae_route_ex.int_time[i];
        ae_route_ex.route_ex_node[i].a_gain = get_pipe_params()[index].static_ae_route_ex.again[i];
        ae_route_ex.route_ex_node[i].d_gain = get_pipe_params()[index].static_ae_route_ex.dgain[i];
        ae_route_ex.route_ex_node[i].isp_d_gain = get_pipe_params()[index].static_ae_route_ex.isp_dgain[i];
    }
    ret = ot_eis_vi_ae_set_route_ex_attr(pipe_hdl, &ae_route_ex);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_eis_vi_ae_get_exposure_attr(pipe_hdl, &exposure_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    exposure_attr.ae_route_ex_valid = get_pipe_params()[index].static_ae.ae_route_ex_valid;
    exposure_attr.ae_run_interval = get_pipe_params()[index].static_ae.ae_run_interval;
    exposure_attr.auto_attr.sys_gain_range.max = get_pipe_params()[index].static_ae.auto_sys_gain_max;
    exposure_attr.auto_attr.exp_time_range.max = get_pipe_params()[index].static_ae.auto_exp_time_max;
    exposure_attr.auto_attr.speed = get_pipe_params()[index].static_ae.auto_speed;
    exposure_attr.auto_attr.tolerance = get_pipe_params()[index].static_ae.auto_tolerance;
    exposure_attr.auto_attr.ae_delay_attr.black_delay_frame = get_pipe_params()[index].static_ae.auto_black_delay_frame;
    exposure_attr.auto_attr.ae_delay_attr.white_delay_frame = get_pipe_params()[index].static_ae.auto_white_delay_frame;
    exposure_attr.auto_attr.antiflicker.enable = get_pipe_params()[index].static_ae.auto_antiflicker_enable;
    exposure_attr.hist_stat_adjust = get_pipe_params()[index].static_ae.hist_stat_adjust;
    exposure_attr.auto_attr.exp_time_range.min = get_pipe_params()[index].static_ae.auto_exp_time_min;

    exposure_attr.auto_attr.a_gain_range.min = get_pipe_params()[index].static_ae.a_gain_min;
    exposure_attr.auto_attr.a_gain_range.max = get_pipe_params()[index].static_ae.a_gain_max;
    exposure_attr.auto_attr.d_gain_range.min = get_pipe_params()[index].static_ae.d_gain_min;
    exposure_attr.auto_attr.d_gain_range.max = get_pipe_params()[index].static_ae.d_gain_max;
    exposure_attr.auto_attr.ispd_gain_range.min = get_pipe_params()[index].static_ae.isp_d_gain_min;
    exposure_attr.auto_attr.ispd_gain_range.max = get_pipe_params()[index].static_ae.isp_d_gain_max;
    exposure_attr.auto_attr.compensation = get_pipe_params()[index].static_ae.compensation;

    ret = ot_eis_vi_ae_set_exposure_attr(pipe_hdl, &exposure_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    if (get_pipe_params()[index].module_state.ae_weight_tab) {
        ret = ot_scene_set_ae_weight_table(pipe_hdl, index);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    }
	scene_logd("index:%d default frame rate:%f\n", index, pub_attr.frame_rate);
    g_fps_set_target[master_pipe_id] = get_pipe_params()[index].static_ae.frame_rate;
    pub_attr.frame_rate = (((float)g_fps_set_target[master_pipe_id])/100); //base 100,int -> float

    scene_logd("index:%d setting frame rate:%f\n", index, pub_attr.frame_rate);

    ret = ot_eis_vi_public_set_attr(pipe_hdl, &pub_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_deflicker_param(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_bool enable)
{
    //check
    ot_s32 ret;
    ot_eis_vi_exposure_attr exposure_attr;

    ret = ot_eis_vi_ae_get_exposure_attr(pipe_hdl, &exposure_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    exposure_attr.auto_attr.antiflicker.enable = enable;

    ret = ot_eis_vi_ae_set_exposure_attr(pipe_hdl, &exposure_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}


ot_s32 ot_scene_set_static_gamma(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_gamma != OT_TRUE) {
        return OT_SUCCESS;
    }
    ot_s32 ret;
    ot_eis_vi_gamma_attr isp_gamma_attr;

    ret = ot_eis_vi_gamma_get_attr(pipe_hdl, &isp_gamma_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    isp_gamma_attr.enable = get_pipe_params()[index].static_gamma.gamma_enable;

    for (int j = 0; j < OT_EIS_VI_GAMMA_NODE_NUM; j++) {
        isp_gamma_attr.table[j] = get_pipe_params()[index].static_gamma.gamma_table[j];
    }

    isp_gamma_attr.curve_type = get_pipe_params()[index].static_gamma.curve_type;

    ret = ot_eis_vi_gamma_set_attr(pipe_hdl, &isp_gamma_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}


ot_s32 ot_scene_set_static_wdr_exposure(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_wdr_exposure != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, ret;
    ot_eis_vi_wdr_exposure_attr wdr_exposure_attr;

    ret = ot_eis_vi_ae_get_wdr_exposure_attr(pipe_hdl, &wdr_exposure_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    wdr_exposure_attr.exp_ratio_type = (ot_eis_op_mode)get_pipe_params()[index].static_wdr_exposure.exp_ratio_type;
    wdr_exposure_attr.exp_ratio_max = get_pipe_params()[index].static_wdr_exposure.exp_ratio_max;
    wdr_exposure_attr.exp_ratio_min = get_pipe_params()[index].static_wdr_exposure.exp_ratio_min;
    wdr_exposure_attr.tolerance = get_pipe_params()[index].static_wdr_exposure.tolerance;
    wdr_exposure_attr.high_light_target = get_pipe_params()[index].static_wdr_exposure.high_light_target;
    wdr_exposure_attr.exp_coef_min = get_pipe_params()[index].static_wdr_exposure.exp_coef_min;

    for (i = 0; i < OT_EIS_VI_EXP_RATIO_NUM; i++) {
        wdr_exposure_attr.exp_ratio[i] = get_pipe_params()[index].static_wdr_exposure.exp_ratio[i];
    }

    ret = ot_eis_vi_ae_set_wdr_exposure_attr(pipe_hdl, &wdr_exposure_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_fswdr(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    if (get_pipe_params()[index].module_state.static_fswdr != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_vi_wdr_attr fswdr_attr;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_wdr_get_attr(pipe_hdl, &fswdr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    fswdr_attr.wdr_merge_mode = get_pipe_params()[index].static_fswdr.wdr_merge_mode;
    fswdr_attr.fusion_attr.fusion_blend_en = get_pipe_params()[index].static_fswdr.fusion_blend_en;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_wdr_set_attr(pipe_hdl, &fswdr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_awb(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_awb != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, ret;
    ot_eis_vi_awb_attr wb_attr;
    ot_eis_vi_stats_cfg isp_stats_cfg;
    ot_eis_vi_awb_mode_switch_attr mode_switch_attr;
    ot_eis_vi_awb_plck_weight_attr plck_wt_attr;
    ot_eis_vi_awb_ls_reproject_attr rp_attr;

    ret = ot_eis_vi_awb_get_attr(pipe_hdl, &wb_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_stats_get_cfg(pipe_hdl, &isp_stats_cfg);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_awb_get_mode_switch_attr(pipe_hdl, &mode_switch_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_awb_get_plck_wt_attr(pipe_hdl, &plck_wt_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_awb_get_rp_attr(pipe_hdl, &rp_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    wb_attr.op_type = get_pipe_params()[index].static_awb.op_type;
    wb_attr.manual_attr.r_gain = get_pipe_params()[index].static_awb.manual_rgain;
    wb_attr.manual_attr.gr_gain = get_pipe_params()[index].static_awb.manual_grgain;
    wb_attr.manual_attr.gb_gain = get_pipe_params()[index].static_awb.manual_gbgain;
    wb_attr.manual_attr.b_gain = get_pipe_params()[index].static_awb.manual_bgain;

    for (i = 0; i < OT_EIS_VI_BAYER_CHN_NUM; i++) {
        wb_attr.auto_attr.static_wb[i] = get_pipe_params()[index].static_awb.auto_static_wb[i];
    }

    for (i = 0; i < OT_EIS_VI_AWB_CURVE_PARA_NUM; i++) {
        wb_attr.auto_attr.curve_para[i] = get_pipe_params()[index].static_awb.auto_curve_para[i];
    }

    wb_attr.auto_attr.speed = get_pipe_params()[index].static_awb.auto_speed;
    wb_attr.auto_attr.low_color_temp = get_pipe_params()[index].static_awb.auto_low_color_temp;
    wb_attr.auto_attr.high_color_temp = get_pipe_params()[index].static_awb.auto_high_color_temp;

    for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
        wb_attr.auto_attr.cb_cr_track.cr_max[i] = get_pipe_params()[index].static_awb.auto_cr_max[i];
        wb_attr.auto_attr.cb_cr_track.cr_min[i] = get_pipe_params()[index].static_awb.auto_cr_min[i];
        wb_attr.auto_attr.cb_cr_track.cb_max[i] = get_pipe_params()[index].static_awb.auto_cb_max[i];
        wb_attr.auto_attr.cb_cr_track.cb_min[i] = get_pipe_params()[index].static_awb.auto_cb_min[i];
    }
    wb_attr.auto_attr.luma_hist.enable = get_pipe_params()[index].static_awb.luma_hist_enable;
    wb_attr.auto_attr.luma_hist.op_type = get_pipe_params()[index].static_awb.luma_hist_op_type;
    for (i = 0; i < OT_EIS_VI_AWB_LUM_HIST_NUM; i++) {
        wb_attr.auto_attr.luma_hist.hist_thresh[i] = get_pipe_params()[index].static_awb.luma_hist_thresh[i];
        wb_attr.auto_attr.luma_hist.hist_wt[i] = get_pipe_params()[index].static_awb.hist_wt[i];
    }

    isp_stats_cfg.wb_cfg.awb_stats_pos = get_pipe_params()[index].static_awb.awb_switch;
    isp_stats_cfg.wb_cfg.black_level = get_pipe_params()[index].static_awb.black_level;

    wb_attr.awb_run_interval = get_pipe_params()[index].static_awb.awb_run_interval;
    wb_attr.auto_attr.ct_limit.enable = get_pipe_params()[index].static_awb.ct_limit_enable;
    wb_attr.auto_attr.ct_limit.op_type = get_pipe_params()[index].static_awb.ct_limit_op_type;
    wb_attr.auto_attr.ct_limit.high_rg_limit = get_pipe_params()[index].static_awb.high_rg_limit;
    wb_attr.auto_attr.ct_limit.high_bg_limit = get_pipe_params()[index].static_awb.high_bg_limit;
    wb_attr.auto_attr.ct_limit.low_rg_limit = get_pipe_params()[index].static_awb.low_rg_limit;
    wb_attr.auto_attr.ct_limit.low_bg_limit = get_pipe_params()[index].static_awb.low_bg_limit;
    wb_attr.auto_attr.shift_limit = get_pipe_params()[index].static_awb.shift_limit;

    mode_switch_attr.night_exp_thresh = get_pipe_params()[index].static_awb.night_exp_thresh;
    mode_switch_attr.skin_tune_exp_thresh = get_pipe_params()[index].static_awb.skin_office_exp_thresh;
    mode_switch_attr.out_switch_luma_thresh = get_pipe_params()[index].static_awb.out_switch_luma_thresh;
    mode_switch_attr.out_switch_lowct_num = get_pipe_params()[index].static_awb.out_switch_lowct_num;

    plck_wt_attr.enable = get_pipe_params()[index].static_awb.weight_enable;
    plck_wt_attr.strength = get_pipe_params()[index].static_awb.weight_strength;

    rp_attr.enable = get_pipe_params()[index].static_awb.weight_enable;
    for (i = 0; i < OT_EIS_VI_AWB_RP_LS_NUM; i++) {
        rp_attr.rp_valid[i] = get_pipe_params()[index].static_awb.rp_valid[i];
        rp_attr.rp_tgt_rgain[i] = get_pipe_params()[index].static_awb.rp_tgt_rgain[i];
        rp_attr.rp_tgt_bgain[i] = get_pipe_params()[index].static_awb.rp_tgt_bgain[i];
        rp_attr.rp_ls_radius[i] = get_pipe_params()[index].static_awb.rp_ls_radius[i];
        rp_attr.rp_zone_rgain[i] = get_pipe_params()[index].static_awb.rp_zone_rgain[i];
        rp_attr.rp_zone_bgain[i] = get_pipe_params()[index].static_awb.rp_zone_bgain[i];
        rp_attr.rp_boundry_ratio[i] = get_pipe_params()[index].static_awb.rp_boundry_ratio[i];
        rp_attr.rp_exp_range_left[i] = get_pipe_params()[index].static_awb.rp_exp_range_left[i];
        rp_attr.rp_exp_range_right[i] = get_pipe_params()[index].static_awb.rp_exp_range_right[i];
        rp_attr.rp_ct_range_left[i] = get_pipe_params()[index].static_awb.rp_ct_range_left[i];
        rp_attr.rp_ct_range_right[i] = get_pipe_params()[index].static_awb.rp_ct_range_right[i];
    }

    ret = ot_eis_vi_awb_set_attr(pipe_hdl, &wb_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_stats_set_cfg(pipe_hdl, &isp_stats_cfg);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_awb_set_mode_switch_attr(pipe_hdl, &mode_switch_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_awb_set_plck_wt_attr(pipe_hdl, &plck_wt_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_awb_set_rp_attr(pipe_hdl, &rp_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_mce(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_mce != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 ret = ot_eis_scene_mce_set_attr(pipe_hdl, &get_pipe_params()[index].static_mce.mce_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_awbex(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_awbex != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, ret;
    ot_eis_vi_awb_attr wb_attr;
    ot_eis_vi_awb_ex_attr awb_attr_ex;

    ret = ot_eis_vi_awb_get_attr(pipe_hdl, &wb_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    wb_attr.bypass = get_pipe_params()[index].static_awb_ex.bypass;
    ret = ot_eis_vi_awb_set_attr(pipe_hdl, &wb_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_eis_vi_awb_get_ex_attr(pipe_hdl, &awb_attr_ex);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    awb_attr_ex.tolerance = get_pipe_params()[index].static_awb_ex.tolerance;
    awb_attr_ex.in_or_out.enable = get_pipe_params()[index].static_awb_ex.in_or_out_enable;
    awb_attr_ex.in_or_out.op_type = get_pipe_params()[index].static_awb_ex.in_or_out_op_type;
    awb_attr_ex.in_or_out.scene_status = get_pipe_params()[index].static_awb_ex.in_or_out_outdoor_status;
    awb_attr_ex.in_or_out.green_enhance_en = get_pipe_params()[index].static_awb_ex.in_or_out_green_enhance_en;
    awb_attr_ex.in_or_out.out_shift_limit = get_pipe_params()[index].static_awb_ex.out_shift_limit;
    awb_attr_ex.in_or_out.out_thresh = get_pipe_params()[index].static_awb_ex.out_thresh;
	awb_attr_ex.in_or_out.low_start = get_pipe_params()[index].static_awb_ex.low_start;
    awb_attr_ex.in_or_out.low_stop = get_pipe_params()[index].static_awb_ex.low_stop;
    awb_attr_ex.in_or_out.high_start = get_pipe_params()[index].static_awb_ex.high_start;
    awb_attr_ex.in_or_out.high_stop = get_pipe_params()[index].static_awb_ex.high_stop;
    awb_attr_ex.multi_light_source_en = get_pipe_params()[index].static_awb_ex.multi_light_source_en;

    awb_attr_ex.zone_radius = get_pipe_params()[index].static_awb_ex.zone_radius;
    awb_attr_ex.curve_l_limit = get_pipe_params()[index].static_awb_ex.curve_l_limit;
    awb_attr_ex.curve_r_limit = get_pipe_params()[index].static_awb_ex.curve_r_limit;
    awb_attr_ex.extra_light_en = get_pipe_params()[index].static_awb_ex.extra_light_en;

    for (i = 0; i < OT_EIS_VI_AWB_LS_NUM; i++) {
        awb_attr_ex.light_info[i].white_r_gain = get_pipe_params()[index].static_awb_ex.white_r_gain[i];
        awb_attr_ex.light_info[i].white_b_gain = get_pipe_params()[index].static_awb_ex.white_b_gain[i];
        awb_attr_ex.light_info[i].exp_quant = get_pipe_params()[index].static_awb_ex.exp_quant[i];
        awb_attr_ex.light_info[i].light_status = get_pipe_params()[index].static_awb_ex.light_status[i];
        awb_attr_ex.light_info[i].radius = get_pipe_params()[index].static_awb_ex.radius[i];
    }

    awb_attr_ex.fine_tun_en = get_pipe_params()[index].static_awb_ex.fine_tun_en;
    awb_attr_ex.fine_tun_strength = get_pipe_params()[index].static_awb_ex.fine_tun_strength;


    for (i = 0; i < OT_EIS_VI_AWB_MULTI_CT_NUM; i++) {
        awb_attr_ex.multi_ct_wt[i] = get_pipe_params()[index].static_awb_ex.multi_ctwt[i];
    }
    ret = ot_eis_vi_awb_set_ex_attr(pipe_hdl, &awb_attr_ex);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_saturation(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    if (get_pipe_params()[index].module_state.static_saturation != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, ret;
    ot_eis_vi_saturation_attr saturation_attr;

    ret = ot_eis_vi_awb_get_saturation_attr(pipe_hdl, &saturation_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
        saturation_attr.auto_attr.sat[i] = get_pipe_params()[index].static_saturation.auto_sat[i];
    }
    ret = ot_eis_vi_awb_set_saturation_attr(pipe_hdl, &saturation_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_ccm(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_ccm != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_u32 i, j, ret;
    ot_eis_vi_color_matrix_attr color_matrix_attr;

    ret = ot_eis_vi_awb_get_ccm_attr(pipe_hdl, &color_matrix_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    color_matrix_attr.op_type = (ot_eis_op_mode)get_pipe_params()[index].static_ccm.ccm_op_type;

    for (i = 0; i < OT_EIS_VI_CCM_MATRIX_SIZE; i++) {
        color_matrix_attr.manual_attr.ccm[i] = get_pipe_params()[index].static_ccm.manual_ccm[i];
    }
    color_matrix_attr.auto_attr.iso_act_en = get_pipe_params()[index].static_ccm.auto_iso_act_en;
    color_matrix_attr.auto_attr.temp_act_en = get_pipe_params()[index].static_ccm.auto_temp_act_en;
    color_matrix_attr.auto_attr.ccm_tab_num = (ot_u16)get_pipe_params()[index].static_ccm.total_num;

    for (i = 0; i < get_pipe_params()[index].static_ccm.total_num; i++) {
        color_matrix_attr.auto_attr.ccm_tab[i].color_temp = get_pipe_params()[index].static_ccm.auto_color_temp[i];
        for (j = 0; j < OT_EIS_VI_CCM_MATRIX_SIZE; j++) {
            color_matrix_attr.auto_attr.ccm_tab[i].ccm[j] = get_pipe_params()[index].static_ccm.auto_ccm[i][j];
        }
    }
    ret = ot_eis_vi_awb_set_ccm_attr(pipe_hdl, &color_matrix_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}


ot_s32 ot_scene_set_static_cac(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_cac != OT_TRUE) {
        return OT_SUCCESS;
    }
    ot_u32 i, j;
    ot_s32 ret;
    ot_eis_vi_cac_attr cac_attr;

    ret = ot_eis_vi_cac_get_attr(pipe_hdl, &cac_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    cac_attr.enable = get_pipe_params()[index].static_cac.enable;
    cac_attr.op_type = get_pipe_params()[index].static_cac.op_type;
    cac_attr.detect_mode = get_pipe_params()[index].static_cac.detect_mode;
    cac_attr.purple_lower_limit = get_pipe_params()[index].static_cac.purple_lower_limit;
    cac_attr.purple_upper_limit = get_pipe_params()[index].static_cac.purple_upper_limit;
    cac_attr.lcac_cfg.purple_detect_range = get_pipe_params()[index].static_cac.purple_detect_range;
    cac_attr.lcac_cfg.var_threshold = get_pipe_params()[index].static_cac.var_threshold;

    for (i = 0; i < OT_SCENE_ACAC_THR_NUM; i++) {
        for (j = 0; j < OT_EIS_VI_AUTO_ISO_NUM; j++) {
            cac_attr.acac_cfg.acac_auto.edge_threshold[i][j] =
                get_pipe_params()[index].static_cac.edge_threshold[i][j];
        }
    }

    for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
        cac_attr.acac_cfg.acac_auto.edge_gain[i] = get_pipe_params()[index].static_cac.edge_gain[i];
        cac_attr.acac_cfg.acac_auto.purple_alpha[i] = get_pipe_params()[index].static_cac.purple_alpha[i];
        cac_attr.acac_cfg.acac_auto.edge_alpha[i] = get_pipe_params()[index].static_cac.edge_alpha[i];
        cac_attr.acac_cfg.acac_auto.cac_rb_strength[i] = get_pipe_params()[index].static_cac.cac_rb_strength[i];
        cac_attr.acac_cfg.acac_auto.satu_low_threshold[i] =
            get_pipe_params()[index].static_cac.satu_low_threshold[i];
        cac_attr.acac_cfg.acac_auto.satu_high_threshold[i] =
            get_pipe_params()[index].static_cac.satu_high_threshold[i];
    }

    for (i = 0; i < OT_EIS_VI_CAC_CURVE_NUM; i++) {
        cac_attr.lcac_cfg.r_detect_threshold[i] = get_pipe_params()[index].static_cac.r_detect_threshold[i];
        cac_attr.lcac_cfg.g_detect_threshold[i] = get_pipe_params()[index].static_cac.g_detect_threshold[i];
        cac_attr.lcac_cfg.b_detect_threshold[i] = get_pipe_params()[index].static_cac.b_detect_threshold[i];
    }

    for (i = 0; i < OT_EIS_VI_CAC_EXP_RATIO_NUM; i++) {
        cac_attr.lcac_cfg.lcac_auto.de_purple_cb_strength[i] =
           get_pipe_params()[index].static_cac.de_purple_cb_strength[i];
        cac_attr.lcac_cfg.lcac_auto.de_purple_cr_strength[i] =
           get_pipe_params()[index].static_cac.de_purple_cr_strength[i];
    }

    ret = ot_eis_vi_cac_set_attr(pipe_hdl, &cac_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}


ot_s32 ot_scene_set_static_dpc(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_dpc != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, ret;
    ot_s32 j;
    ot_eis_vi_dpc_dynamic_cor_attr dp_dynamic_attr;

    ret = ot_eis_vi_dpc_get_dynamic_cor_attr(pipe_hdl, &dp_dynamic_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    dp_dynamic_attr.enable = get_pipe_params()[index].static_dpc.enable;

    for (j = 0; j < OT_EIS_VI_WDR_MAX_FRAME_NUM; j++) {
        dp_dynamic_attr.frame_dynamic[j].op_type = get_pipe_params()[index].static_dpc.op_type[j];
    }

    for (j = 0; j < OT_EIS_VI_WDR_MAX_FRAME_NUM; j++) {
        for (i = 0; i < OT_EIS_SCENE_ISO_STRENGTH_NUM; i++) {
            dp_dynamic_attr.frame_dynamic[j].auto_attr.strength[i] = get_pipe_params()[index].static_dpc.strength[j][i];
            dp_dynamic_attr.frame_dynamic[j].auto_attr.blend_ratio[i] =
                get_pipe_params()[index].static_dpc.blend_ratio[j][i];
        }
    }

    ret = ot_eis_vi_dpc_set_dynamic_cor_attr(pipe_hdl, &dp_dynamic_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}


ot_s32 ot_scene_set_static_drc(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_drc != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, ret;
    ot_eis_vi_drc_attr drc_attr;

    ret = ot_eis_vi_drc_get_attr(pipe_hdl, &drc_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    drc_attr.enable = get_pipe_params()[index].static_drc.enable;
    drc_attr.curve_select = (ot_eis_vi_drc_curve_select)get_pipe_params()[index].static_drc.curve_select;
    drc_attr.op_type = (ot_eis_op_mode)get_pipe_params()[index].static_drc.op_type;
    drc_attr.purple_reduction_strength = get_pipe_params()[index].static_drc.purple_reduction_strength;

    drc_attr.contrast_ctrl = get_pipe_params()[index].static_drc.contrast_ctrl;
    /* blend_luma_max */
    /* blend_luma_bright_min */
    /* blend_luma_bright_threshold */
    /* blend_luma_dark_min */
    /* blend_luma_dark_threshold */
    /* blend_detail_max */
    /* blend_detail_bright_min */
    /* blend_detail_bright_threshold */
    /* blend_detail_dark_min */
    /* blend_detail_dark_threshold */
    drc_attr.global_color_ctrl = get_pipe_params()[index].static_drc.global_color_ctrl;

    for (i = 0; i < OT_EIS_VI_DRC_CC_NODE_NUM; i++) {
        drc_attr.color_correction_lut[i] = get_pipe_params()[index].static_drc.color_correction_lut[i];
    }

    ret = ot_eis_vi_drc_set_attr(pipe_hdl, &drc_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}


ot_s32 ot_scene_set_static_ldci(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_ldci != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, ret;
    ot_eis_vi_ldci_attr ldci_attr;

    ret = ot_eis_vi_ldci_get_attr(pipe_hdl, &ldci_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ldci_attr.enable = get_pipe_params()[index].static_ldci.enable;
    ldci_attr.op_type = (ot_eis_op_mode)get_pipe_params()[index].static_ldci.ldci_op_type;
    ldci_attr.gauss_lpf_sigma = get_pipe_params()[index].static_ldci.gauss_lpf_sigma;
    ldci_attr.tpr_incr_coef = get_pipe_params()[index].static_ldci.tpr_incr_coef;
    ldci_attr.tpr_decr_coef = get_pipe_params()[index].static_ldci.tpr_decr_coef;

    for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
        ldci_attr.auto_attr.he_wgt[i].he_pos_wgt.sigma = get_pipe_params()[index].static_ldci.auto_he_pos_sigma[i];
        ldci_attr.auto_attr.he_wgt[i].he_pos_wgt.wgt = get_pipe_params()[index].static_ldci.auto_he_pos_wgt[i];
        ldci_attr.auto_attr.he_wgt[i].he_pos_wgt.mean = get_pipe_params()[index].static_ldci.auto_he_pos_mean[i];
        ldci_attr.auto_attr.he_wgt[i].he_neg_wgt.sigma = get_pipe_params()[index].static_ldci.auto_he_neg_sigma[i];
        ldci_attr.auto_attr.he_wgt[i].he_neg_wgt.wgt = get_pipe_params()[index].static_ldci.auto_he_neg_wgt[i];
        ldci_attr.auto_attr.he_wgt[i].he_neg_wgt.mean = get_pipe_params()[index].static_ldci.auto_he_neg_mean[i];
        ldci_attr.auto_attr.blc_ctrl[i] = get_pipe_params()[index].static_ldci.auto_blc_ctrl[i];
    }

    ret = ot_eis_vi_ldci_set_attr(pipe_hdl, &ldci_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_ldcihlc(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_ldcihlc != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_vi_ldcihlc_attr ldcihlc_attr;

    ret = ot_eis_vi_ldcihlc_get_attr(pipe_hdl, &ldcihlc_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ldcihlc_attr.enable = get_pipe_params()[index].static_ldcihlc.enable;

    ret = ot_eis_vi_ldcihlc_set_attr(pipe_hdl, &ldcihlc_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

static ot_s32 g_dehaze_str = 32;

ot_s32 ot_scene_set_static_dehaze(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_dehaze != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, ret;
    ot_eis_vi_dehaze_attr dehaze_attr;
    ret = ot_eis_vi_dehaze_get_attr(pipe_hdl, &dehaze_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    dehaze_attr.enable = get_pipe_params()[index].static_dehaze.enable;
    dehaze_attr.op_type = (ot_eis_op_mode)get_pipe_params()[index].static_dehaze.dehaze_op_type;
    dehaze_attr.user_lut_en = get_pipe_params()[index].static_dehaze.user_lut_enable;
    dehaze_attr.tmprflt_incr_coef = get_pipe_params()[index].static_dehaze.tmprflt_incr_coef;
    dehaze_attr.tmprflt_decr_coef = get_pipe_params()[index].static_dehaze.tmprflt_decr_coef;
    dehaze_attr.manual_attr.strength = g_dehaze_str;

    for (i = 0; i < OT_EIS_VI_DEHAZE_LUT_SIZE; i++) {
        dehaze_attr.dehaze_lut[i] = get_pipe_params()[index].static_dehaze.dehaze_lut[i];
    }
    ret = ot_eis_vi_dehaze_set_attr(pipe_hdl, &dehaze_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_dehaze_param(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_bool enable)
{
    //check
    ot_s32 ret;
    ot_eis_vi_dehaze_attr dehaze_attr;

    ret = ot_eis_vi_dehaze_get_attr(pipe_hdl, &dehaze_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    dehaze_attr.enable = enable;

    ret = ot_eis_vi_dehaze_set_attr(pipe_hdl, &dehaze_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_csc(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_csc != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_vi_csc_attr csc_attr;

    ret = ot_eis_vi_csc_get_attr(pipe_hdl, &csc_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    csc_attr.enable = get_pipe_params()[index].staic_csc.enable;
    csc_attr.hue = get_pipe_params()[index].staic_csc.hue;
    csc_attr.luma = get_pipe_params()[index].staic_csc.luma;
    csc_attr.contr = get_pipe_params()[index].staic_csc.contrast;
    csc_attr.satu = get_pipe_params()[index].staic_csc.saturation;
    csc_attr.color_gamut = (ot_eis_img_color_space)get_pipe_params()[index].staic_csc.color_gamut;

    ret = ot_eis_vi_csc_set_attr(pipe_hdl, &csc_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

static ot_s32 ot_scene_set_static_nr_snr(ot_u8 index, ot_eis_vi_nr_attr *nr_attr)
{
    ot_s32 i, j;

    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    ot_scenecomm_check_pointer_return(nr_attr, OT_FAILURE);
    for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
        nr_attr->snr_cfg.snr_attr.snr_auto.sfm0_detail_prot[i] =
            get_pipe_params()[index].static_nr.snr_cfg.sfm0_detail_prot[i];
        nr_attr->snr_cfg.snr_attr.snr_auto.fine_strength[i] =
            get_pipe_params()[index].static_nr.snr_cfg.fine_strength[i];
        nr_attr->snr_cfg.snr_attr.snr_auto.coring_wgt[i] =
            get_pipe_params()[index].static_nr.snr_cfg.coring_wgt[i];
        nr_attr->snr_cfg.snr_attr.snr_auto.coring_mot_ratio[i] =
            get_pipe_params()[index].static_nr.snr_cfg.coring_mot_ratio[i];
        nr_attr->snr_cfg.snr_attr.snr_auto.sfm6_strength[i] =
            get_pipe_params()[index].static_nr.snr_cfg.sfm6_strength[i];
        nr_attr->snr_cfg.snr_attr.snr_auto.sfm7_strength[i] =
            get_pipe_params()[index].static_nr.snr_cfg.sfm7_strength[i];
        nr_attr->snr_cfg.snr_attr.snr_auto.sth[i] =
            get_pipe_params()[index].static_nr.snr_cfg.sth[i];
        nr_attr->snr_cfg.snr_attr.snr_auto.sfm1_adp_strength[i] =
            get_pipe_params()[index].static_nr.snr_cfg.sfm1_adp_strength[i];
        nr_attr->snr_cfg.snr_attr.snr_auto.sfm1_strength[i] =
            get_pipe_params()[index].static_nr.snr_cfg.sfm1_strength[i];
        for (j = 0; j < OT_EIS_VI_BAYER_CHN_NUM; j++) {
            nr_attr->snr_cfg.snr_attr.snr_auto.sfm0_coarse_strength[j][i] =
                get_pipe_params()[index].static_nr.snr_cfg.sfm0_coarse_strength[j][i];
        }
    }
    return OT_SUCCESS;
}

static ot_s32 ot_scene_set_static_nr_tnr(ot_u8 index, ot_eis_vi_nr_attr *nr_attr)
{
    ot_s32 i;

    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    ot_scenecomm_check_pointer_return(nr_attr, OT_FAILURE);
    for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
        nr_attr->tnr_cfg.tnr_auto.md_anti_flicker_strength[i] =
            get_pipe_params()[index].static_nr.tnr_cfg.md_anti_flicker_strength[i];
        nr_attr->tnr_cfg.tnr_auto.md_static_ratio[i] = get_pipe_params()[index].static_nr.tnr_cfg.md_static_ratio[i];
        nr_attr->tnr_cfg.tnr_auto.md_motion_ratio[i] = get_pipe_params()[index].static_nr.tnr_cfg.md_motion_ratio[i];
        nr_attr->tnr_cfg.tnr_auto.md_static_fine_strength[i] =
            get_pipe_params()[index].static_nr.tnr_cfg.md_static_fine_strength[i];
        nr_attr->tnr_cfg.tnr_auto.tfs[i] = get_pipe_params()[index].static_nr.tnr_cfg.tfs[i];
        nr_attr->tnr_cfg.tnr_auto.user_define_md[i] = get_pipe_params()[index].static_nr.tnr_cfg.user_define_md[i];
        nr_attr->tnr_cfg.tnr_auto.user_define_slope[i] =
            get_pipe_params()[index].static_nr.tnr_cfg.user_define_slope[i];
        nr_attr->tnr_cfg.tnr_auto.user_define_dark_thresh[i] =
            get_pipe_params()[index].static_nr.tnr_cfg.user_define_dark_thresh[i];
        nr_attr->tnr_cfg.tnr_auto.user_define_color_thresh[i] =
            get_pipe_params()[index].static_nr.tnr_cfg.user_define_color_thresh[i];

        nr_attr->tnr_cfg.tnr_auto.sfr_r[i] = get_pipe_params()[index].static_nr.tnr_cfg.sfr_r[i];
        nr_attr->tnr_cfg.tnr_auto.sfr_g[i] = get_pipe_params()[index].static_nr.tnr_cfg.sfr_g[i];
        nr_attr->tnr_cfg.tnr_auto.sfr_b[i] = get_pipe_params()[index].static_nr.tnr_cfg.sfr_b[i];
    }
    return OT_SUCCESS;
}

static ot_s32 ot_scene_set_static_nr_post_snr(ot_u8 index, ot_eis_vi_nr_attr *nr_attr)
{
    ot_s32 i;

    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    ot_scenecomm_check_pointer_return(nr_attr, OT_FAILURE);
    for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
        nr_attr->dering_cfg.dering_auto.dering_strength[i] =
            get_pipe_params()[index].static_nr.dering_snr_cfg.dering_strength[i];
        nr_attr->dering_cfg.dering_auto.dering_thresh[i] =
            get_pipe_params()[index].static_nr.dering_snr_cfg.dering_thresh[i];
        nr_attr->dering_cfg.dering_auto.dering_static_strength[i] =
            get_pipe_params()[index].static_nr.dering_snr_cfg.dering_static_strength[i];
        nr_attr->dering_cfg.dering_auto.dering_motion_strength[i] =
            get_pipe_params()[index].static_nr.dering_snr_cfg.dering_motion_strength[i];
    }
    return OT_SUCCESS;
}


ot_s32 ot_scene_set_static_nr(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_nr != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, ret;
    ot_eis_vi_nr_attr nr_attr;

    ret = ot_eis_vi_nr_get_attr(pipe_hdl, &nr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    nr_attr.enable = get_pipe_params()[index].static_nr.enable;
    nr_attr.op_type = (ot_eis_op_mode)get_pipe_params()[index].static_nr.op_type;
    nr_attr.tnr_en = get_pipe_params()[index].static_nr.tnr_enable;
    nr_attr.lsc_nr_en = get_pipe_params()[index].static_nr.lsc_nr_enable;
    nr_attr.lsc_ratio1 = get_pipe_params()[index].static_nr.lsc_ratio1;

    for (i = 0; i < OT_EIS_VI_BAYERNR_LUT_LENGTH; i++) {
        nr_attr.coring_ratio[i] = get_pipe_params()[index].static_nr.coring_ratio[i];
    }

    for (i = 0; i < OT_EIS_VI_BAYERNR_LUT_LENGTH1; i++) {
        nr_attr.mix_gain[i] = get_pipe_params()[index].static_nr.mix_gain[i];
    }

    ret = ot_scene_set_static_nr_snr(index, &nr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_nr_tnr(index, &nr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_nr_post_snr(index, &nr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_eis_vi_nr_set_attr(pipe_hdl, &nr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_shading(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_shading != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_vi_shading_attr shading_attr;

    ret = ot_eis_vi_mlsc_get_attr(pipe_hdl, &shading_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    shading_attr.enable = get_pipe_params()[index].static_shading.enable;
    ret = ot_eis_vi_mlsc_set_attr(pipe_hdl, &shading_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

static ot_s32 ot_scene_set_static_sharpen_param_auto(ot_u8 index, ot_eis_vi_sharpen_attr *sharpen_attr)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    ot_scenecomm_check_pointer_return(sharpen_attr, OT_FAILURE);
    ot_s32 i;

    for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
        sharpen_attr->auto_attr.texture_freq[i] =
            get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.texture_freq[i];
        sharpen_attr->auto_attr.edge_freq[i] = get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.edge_freq[i];
        sharpen_attr->auto_attr.over_shoot[i] = get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.over_shoot[i];
        sharpen_attr->auto_attr.under_shoot[i] =
            get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.under_shoot[i];
        sharpen_attr->auto_attr.shoot_sup_strength[i] =
            get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.shoot_sup_strength[i];
        sharpen_attr->auto_attr.shoot_sup_adj[i] =
            get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.shoot_sup_adj[i];
        sharpen_attr->auto_attr.detail_ctrl[i] =
            get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.detail_ctrl[i];
        sharpen_attr->auto_attr.detail_ctrl_threshold[i] =
            get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.detail_ctrl_threshold[i];
        sharpen_attr->auto_attr.edge_filt_strength[i] =
            get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.edge_filt_strength[i];
        sharpen_attr->auto_attr.edge_filt_max_cap[i] =
            get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.edge_filt_max_cap[i];
        sharpen_attr->auto_attr.r_gain[i] = get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.r_gain[i];
        sharpen_attr->auto_attr.g_gain[i] = get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.g_gain[i];
        sharpen_attr->auto_attr.b_gain[i] = get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.b_gain[i];
        sharpen_attr->auto_attr.skin_gain[i] = get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.skin_gain[i];
        sharpen_attr->auto_attr.max_sharp_gain[i] =
            get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.max_sharp_gain[i];
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_sharpen(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_sharpen != OT_TRUE) {
        return OT_SUCCESS;
    }
    ot_s32 i, j, ret;
    ot_eis_vi_sharpen_attr sharpen_attr;

    ret = ot_eis_vi_yuv_sharpen_get_attr(pipe_hdl, &sharpen_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    sharpen_attr.enable = get_pipe_params()[index].static_sharpen.enable;
    sharpen_attr.skin_umin = get_pipe_params()[index].static_sharpen.skin_umin;
    sharpen_attr.skin_vmin = get_pipe_params()[index].static_sharpen.skin_vmin;
    sharpen_attr.skin_umax = get_pipe_params()[index].static_sharpen.skin_umax;
    sharpen_attr.skin_vmax = get_pipe_params()[index].static_sharpen.skin_vmax;

    for (i = 0; i < OT_EIS_VI_SHARPEN_GAIN_NUM; i++) {
        for (j = 0; j < OT_EIS_VI_AUTO_ISO_NUM; j++) {
            sharpen_attr.auto_attr.luma_wgt[i][j] =
                get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.luma_wgt[i][j];
            sharpen_attr.auto_attr.texture_strength[i][j] =
                get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.texture_strength[i][j];
            sharpen_attr.auto_attr.edge_strength[i][j] =
                get_pipe_params()[index].static_sharpen.sharpen_auto_cfg.edge_strength[i][j];
        }
    }

    ret = ot_scene_set_static_sharpen_param_auto(index, &sharpen_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_eis_vi_yuv_sharpen_set_attr(pipe_hdl, &sharpen_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_demosaic(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_dm != OT_TRUE) {
        return OT_SUCCESS;
    }
    ot_s32 i, ret;
    ot_eis_vi_demosaic_attr dm_attr;

    ret = ot_eis_vi_demosaic_get_attr(pipe_hdl, &dm_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    dm_attr.enable = get_pipe_params()[index].static_dm.enable;
    dm_attr.op_type = get_pipe_params()[index].static_dm.op_type;

    for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
        dm_attr.auto_attr.nddm_strength[i] = get_pipe_params()[index].static_dm.dm_auto_cfg.nddm_strength[i];
        dm_attr.auto_attr.nddm_mf_detail_strength[i] =
            get_pipe_params()[index].static_dm.dm_auto_cfg.nddm_mf_detail_strength[i];
        dm_attr.auto_attr.nddm_hf_detail_strength[i] =
            get_pipe_params()[index].static_dm.dm_auto_cfg.nddm_hf_detail_strength[i];
        dm_attr.auto_attr.detail_smooth_range[i] =
            get_pipe_params()[index].static_dm.dm_auto_cfg.detail_smooth_range[i];
        dm_attr.auto_attr.color_noise_f_threshold[i] =
            get_pipe_params()[index].static_dm.dm_auto_cfg.color_noise_f_threshold[i];
        dm_attr.auto_attr.color_noise_f_strength[i] =
            get_pipe_params()[index].static_dm.dm_auto_cfg.color_noise_f_strength[i];
        dm_attr.auto_attr.color_noise_y_threshold[i] =
            get_pipe_params()[index].static_dm.dm_auto_cfg.color_noise_y_threshold[i];
        dm_attr.auto_attr.color_noise_y_strength[i] =
            get_pipe_params()[index].static_dm.dm_auto_cfg.color_noise_y_strength[i];
    }

    ret = ot_eis_vi_demosaic_set_attr(pipe_hdl, &dm_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_bayershp(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_bayeshp != OT_TRUE) {
        return OT_SUCCESS;
    }
    ot_s32 i, j, ret;
    ot_eis_vi_bayer_sharpen_attr bshp_attr;

    ret = ot_eis_vi_bayer_sharpen_get_attr(pipe_hdl, &bshp_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    bshp_attr.enable = get_pipe_params()[index].static_bayershp.enable;
    bshp_attr.op_type = get_pipe_params()[index].static_bayershp.op_type;

    for (j = 0; j < OT_EIS_VI_BSHP_THD_NUM; j++) {
        bshp_attr.dark_threshold[j] = get_pipe_params()[index].static_bayershp.dark_threshold[j];
        bshp_attr.texture_threshold[j] = get_pipe_params()[index].static_bayershp.texture_threshold[j];
    }

    for (i = 0; i < OT_EIS_VI_BSHP_CURVE_NUM; i++) {
        for (j = 0; j < OT_EIS_VI_AUTO_ISO_NUM; j++) {
            bshp_attr.auto_attr.mf_strength[i][j] =
                get_pipe_params()[index].static_bayershp.bayershp_auto_attr.mf_strength[i][j];
            bshp_attr.auto_attr.hf_strength[i][j] =
                get_pipe_params()[index].static_bayershp.bayershp_auto_attr.hf_strength[i][j];
            bshp_attr.auto_attr.dark_strength[i][j] =
                get_pipe_params()[index].static_bayershp.bayershp_auto_attr.dark_strength[i][j];
        }
    }

    for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
        bshp_attr.auto_attr.mf_gain[i] =
            get_pipe_params()[index].static_bayershp.bayershp_auto_attr.mf_gain[i];
        bshp_attr.auto_attr.hf_gain[i] =
            get_pipe_params()[index].static_bayershp.bayershp_auto_attr.hf_gain[i];
        bshp_attr.auto_attr.overshoot[i] = get_pipe_params()[index].static_bayershp.bayershp_auto_attr.overshoot[i];
        bshp_attr.auto_attr.undershoot[i] = get_pipe_params()[index].static_bayershp.bayershp_auto_attr.undershoot[i];
        bshp_attr.auto_attr.dark_gain[i] = get_pipe_params()[index].static_bayershp.bayershp_auto_attr.dark_gain[i];
    }

    ret = ot_eis_vi_bayer_sharpen_set_attr(pipe_hdl, &bshp_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_ca(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_ca != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, ret;
    ot_eis_vi_ca_attr ca_attr;

    ret = ot_eis_vi_ca_get_attr(pipe_hdl, &ca_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ca_attr.enable = get_pipe_params()[index].static_ca.enable;
    for (i = 0; i < ISP_AUTO_ISO_CA_NUM; i++) {
        ca_attr.ca.iso_ratio[i] = get_pipe_params()[index].static_ca.iso_ratio[i];
    }

    for (i = 0; i < OT_EIS_VI_CA_YRATIO_LUT_LENGTH; i++) {
        ca_attr.ca.y_ratio_lut[i] = get_pipe_params()[index].static_ca.y_ratio_lut[i];
    }

    for (i = 0; i < OT_EIS_VI_CA_YRATIO_LUT_LENGTH; i++) {
        ca_attr.ca.y_sat_lut[i] = get_pipe_params()[index].static_ca.y_sat_lut[i];
    }

    ret = ot_eis_vi_ca_set_attr(pipe_hdl, &ca_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}


ot_s32 ot_scene_set_static_crosstalk(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_cross_talk != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_vi_cr_attr crosstalk_attr;

    ret = ot_eis_vi_crosstalk_removal_get_attr(pipe_hdl, &crosstalk_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    crosstalk_attr.enable = get_pipe_params()[index].static_crosstalk.enable;
    ret = ot_eis_vi_crosstalk_removal_set_attr(pipe_hdl, &crosstalk_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_clut(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    if (get_pipe_params()[index].module_state.static_clut != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_vi_clut_attr clut_attr;
    ot_eis_vi_clut_lut clut_lut;

    ret = ot_eis_vi_clut_get_attr(pipe_hdl, &clut_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    clut_attr.enable = get_pipe_params()[index].static_clut.enable;


    ret = ot_eis_vi_clut_get_coeff(pipe_hdl, &clut_lut);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    if (get_pipe_params()[index].static_clut.lut_grp < 4) {
        ot_u8 table_index = get_pipe_params()[index].static_clut.lut_grp - 1;
        clut_attr.gain_r = get_pipe_params()[index].static_clut.clut_table[table_index].gain[0];
        clut_attr.gain_g = get_pipe_params()[index].static_clut.clut_table[table_index].gain[1];
        clut_attr.gain_b = get_pipe_params()[index].static_clut.clut_table[table_index].gain[2];

        memcpy_s(clut_lut.lut, sizeof(ot_u32) * OT_EIS_VI_CLUT_LUT_LENGTH,
                &(get_pipe_params()[index].static_clut.clut_table[table_index].lut), sizeof(ot_u32) * OT_EIS_VI_CLUT_LUT_LENGTH);
    } else {
        scene_loge("unvalid clut grp\n");
        (ot_void)memset_s(clut_lut.lut, sizeof(ot_u32) * OT_EIS_VI_CLUT_LUT_LENGTH, 0, sizeof(ot_u32) * OT_EIS_VI_CLUT_LUT_LENGTH);
    }

    ret = ot_eis_vi_clut_set_attr(pipe_hdl, &clut_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_eis_vi_clut_set_coeff(pipe_hdl, &clut_lut);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_clut_param(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u8 ct_value)
{
    //check ct_value range [0,100]
    if (ct_value > 10) {
        scene_loge("unvalid ct_value\n");
        return OT_FAILURE;
    }

    ot_s32 ret;
    ot_eis_vi_clut_attr clut_attr;

    ret = ot_eis_vi_clut_get_attr(pipe_hdl, &clut_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    clut_attr.gain_r = (get_pipe_params()[index].static_clut.clut_table[0].gain[0] * ct_value)/10;
    clut_attr.gain_g = (get_pipe_params()[index].static_clut.clut_table[0].gain[1] * ct_value)/10;
    clut_attr.gain_b = (get_pipe_params()[index].static_clut.clut_table[0].gain[2] * ct_value)/10;

    ret = ot_eis_vi_clut_set_attr(pipe_hdl, &clut_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_blc(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_blc != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, j, ret;
    ot_eis_vi_black_level_attr black_level_attr;

    ret = ot_eis_vi_black_level_get_attr(pipe_hdl, &black_level_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    black_level_attr.user_black_level_en = get_pipe_params()[index].static_blc.enable;

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        for (j = 0; j < OT_EIS_VI_BAYER_CHN_NUM; j++) {
            black_level_attr.user_black_level[i][j] = get_pipe_params()[index].static_blc.user_offset;
        }
    }
    ret = ot_eis_vi_black_level_set_attr(pipe_hdl, &black_level_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}


ot_s32 ot_scene_set_static_3dnr(ot_eis_handle vi_pipe_hdl, ot_eis_handle vproc_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    ot_scene_3dnr_param param = {0};
    param.enable = get_pipe_params()[index].static_threednr.enable;
    return ot_scene_set_static_3dnr_param(vi_pipe_hdl, vproc_hdl, &param, index);
}


ot_s32 ot_scene_set_static_3dnr_param(ot_eis_handle vi_pipe_hdl, ot_eis_handle vproc_hdl, ot_scene_3dnr_param *param, ot_u8 index)
{
    //check
#ifdef SET_3DNR
    ot_scenecomm_expr_true_return(param == OT_NULL, OT_FAILURE);
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_3dnr != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_3dnr_attr nrx_attr = {0};
    ot_u32 param_iso[16] = {0};
    ot_eis_nr_cfg nr_param[16] = {0};

    nrx_attr.param.nr_cfg.nr_auto.iso = param_iso;
    nrx_attr.param.nr_cfg.nr_auto.nr_param = nr_param;

    ot_eis_3dnr_pos pos_3dnr = OT_EIS_3DNR_POS_VI;
    ret = ot_eis_media_get_3dnr_pos(&pos_3dnr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    if (pos_3dnr == OT_EIS_3DNR_POS_VI) {
        ret = ot_eis_vi_pipe_get_3dnr_attr(vi_pipe_hdl,  &nrx_attr);
    } else {
        nrx_attr.param.nr_cfg.nr_mode = OT_EIS_OP_MODE_MANUAL;
        ret = ot_eis_vproc_grp_get_3dnr(vproc_hdl,  &nrx_attr);
    }

    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    nrx_attr.enable = param->enable;
    if (param->set_compress_mode == OT_TRUE) {
        nrx_attr.compress_mode = param->compress_mode;
    }

    if (pos_3dnr == OT_EIS_3DNR_POS_VI) {
        ret = ot_eis_vi_pipe_set_3dnr_attr(vi_pipe_hdl,  &nrx_attr);
    } else {
        ret = ot_eis_vproc_grp_set_3dnr(vproc_hdl,  &nrx_attr);
    }

    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

#endif
    return OT_SUCCESS;

}

ot_s32 ot_scene_set_dynamic_ldci(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_ldci != OT_TRUE) || (last_exposure == exposure)) {
        return OT_SUCCESS;
    }

    ot_u32 exp_level;
    ot_s32 ret;
    ot_eis_vi_ldci_attr ldci_attr;

    if (exposure != last_exposure) {
        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_ldci_get_attr(pipe_hdl, &ldci_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

        exp_level = scene_get_level_ltoh(exposure, get_pipe_params()[index].dynamic_ldci.enable_cnt,
            get_pipe_params()[index].dynamic_ldci.enable_exp_thresh_ltoh);
        ot_scenecomm_expr_true_return(exp_level >= OT_SCENE_LDCI_EXPOSURE_MAX_COUNT, OT_FAILURE);
        ldci_attr.enable = get_pipe_params()[index].dynamic_ldci.enable[exp_level];

        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_ldci_set_attr(pipe_hdl, &ldci_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    }
    return OT_SUCCESS;
}


ot_s32 ot_scene_set_dynamic_ldcihlc(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_ldcihlc != OT_TRUE) || (last_exposure == exposure)) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_vi_ldcihlc_attr ldcihlc_attr;

    if (exposure != last_exposure) {
        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_ldcihlc_get_attr(pipe_hdl, &ldcihlc_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

        ldcihlc_attr.threshold = get_pipe_params()[index].dynamic_ldcihlc.threshold;
        ldcihlc_attr.op_type = get_pipe_params()[index].dynamic_ldcihlc.op_type;
        ldcihlc_attr.manual_attr.strength = get_pipe_params()[index].dynamic_ldcihlc.strength_manual;

    for (int i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
        ldcihlc_attr.auto_attr.strength[i] = get_pipe_params()[index].dynamic_ldcihlc.strength_auto[i];
    }

        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_ldcihlc_set_attr(pipe_hdl, &ldcihlc_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_fps(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.dynamic_fps != OT_TRUE) {
        return OT_SUCCESS;
    }
    ot_s32 ret;
    ot_u8 cnt;
    ot_u8 exp_level = 0;
    ot_u64 fps_target;
    ot_eis_vi_public_attr pub_attr;
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_eis_vi_public_get_attr(pipe_hdl, &pub_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    cnt = get_pipe_params()[index].dynamic_fps.fps_exposure_cnt;

    if(exposure == last_exposure) {
        return OT_SUCCESS;
    } else if(exposure > last_exposure) {
        if(exposure >= get_pipe_params()[index].dynamic_fps.exp_ltoh_thresh[cnt - 1]) {
            exp_level = cnt - 1;
        } else {
            for(int i = 0; i < cnt ; i++) {
                if(exposure < get_pipe_params()[index].dynamic_fps.exp_ltoh_thresh[i]) {
                    if(i == 0) {
                        scene_loge("dynamic_fps min ltoh exp not valid\n");
                        return OT_FAILURE;
                    } else {
                        exp_level = i - 1;
                        break;
                    }
                }
            }
        }
    } else if(exposure < last_exposure) {
        if(exposure >= get_pipe_params()[index].dynamic_fps.exp_htol_thresh[cnt - 1]) {
            exp_level = cnt - 1;
        } else {
            for(int i = 0; i < cnt ; i++) {
                if(exposure < get_pipe_params()[index].dynamic_fps.exp_htol_thresh[i]) {
                    if(i == 0) {
                        scene_loge("dynamic_fps min htol exp not valid\n");
                        return OT_FAILURE;
                    } else {
                        exp_level = i;
                        break;
                    }
                }
            }
        }
    }

    fps_target = get_pipe_params()[index].dynamic_fps.fps_set[exp_level];
    if(fps_target != g_fps_set_target[master_pipe_id]) {
        g_fps_set_target[master_pipe_id] = fps_target;
        pub_attr.frame_rate = (((float)g_fps_set_target[master_pipe_id])/100); //base 100,int -> float
        ret = ot_eis_vi_public_set_attr(pipe_hdl, &pub_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_vproc(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index)
{
    // ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.dynamic_vproc != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 i, j;
    ot_s32 ret;
    ot_s32 vi_width,vi_height;
    ot_s32 up_left,up_right,down_left,down_right;
    ot_s32 up_left_awb,up_right_awb,down_left_awb,down_right_awb;
    // ot_s32 inside_count = 0;
    ot_eis_vproc_crop_param vproc_crop_info;
    ot_eis_vi_stats_cfg isp_stats_cfg;
    ot_eis_vi_public_attr vi_public_attr;
    ot_eis_vi_awb_attr wb_attr;
    ot_eis_img_rect rect_ae;
    ot_eis_img_rect rect_awb;
    ot_u32 crop_mode;

    //get crop roi, image size
    crop_mode = get_pipe_params()[index].dynamic_vproc.crop_mode;      //0 for vproc grp, 1 for manual crop

    switch(crop_mode)
    {
        case 0:
            ret = ot_eis_vproc_grp_get_crop(pipe_hdl, &vproc_crop_info);
            check_scene_ret(ret);
            rect_ae.x = vproc_crop_info.crop_rect.x;
            rect_ae.y = vproc_crop_info.crop_rect.y;
            rect_ae.width = vproc_crop_info.crop_rect.width;
            rect_ae.height = vproc_crop_info.crop_rect.height;

            rect_awb.x = vproc_crop_info.crop_rect.x;
            rect_awb.y = vproc_crop_info.crop_rect.y;
            rect_awb.width = vproc_crop_info.crop_rect.width;
            rect_awb.height = vproc_crop_info.crop_rect.height;
            break;

        case 1:
            rect_ae.x = get_pipe_params()[index].dynamic_vproc.rect_ae_x;
            rect_ae.y = get_pipe_params()[index].dynamic_vproc.rect_ae_y;
            rect_ae.width = get_pipe_params()[index].dynamic_vproc.rect_ae_width;
            rect_ae.height = get_pipe_params()[index].dynamic_vproc.rect_ae_height;

            rect_awb.x = get_pipe_params()[index].dynamic_vproc.rect_awb_x;
            rect_awb.y = get_pipe_params()[index].dynamic_vproc.rect_awb_y;
            rect_awb.width = get_pipe_params()[index].dynamic_vproc.rect_awb_width;
            rect_awb.height = get_pipe_params()[index].dynamic_vproc.rect_awb_height;
            break;

        default:
            printf("invalid ae_awb crop mode\n");
            return OT_FAILURE;
            break;
    }

    //get crop roi, stat blks
    ret = ot_eis_vi_public_get_attr(pipe_hdl, &vi_public_attr);
    check_scene_ret(ret);
    vi_width = vi_public_attr.sns_size.width;
    vi_height = vi_public_attr.sns_size.height;

    int calc_1 = 0;
    int calc_2 = 0;

    //AE 17*15
    // >0
    calc_1 = (int)((rect_ae.x*OT_EIS_VI_AE_ZONE_COLUMN)/vi_width);
    up_left = calc_1 > 0 ? calc_1 : 0;
    // min
    calc_1 = OT_EIS_VI_AE_ZONE_COLUMN-1;
    calc_2 = (int)(((rect_ae.x + rect_ae.width)*(OT_EIS_VI_AE_ZONE_COLUMN)+vi_width-1)/vi_width);
    up_right = calc_1 < calc_2 ? calc_1 : calc_2;
    // >0
    calc_1 = (int)((rect_ae.y*OT_EIS_VI_AE_ZONE_ROW)/vi_height);
    down_left = calc_1 > 0 ? calc_1 : 0;
    // min
    calc_1 = OT_EIS_VI_AE_ZONE_ROW-1;
    calc_2 = (int)(((rect_ae.y + rect_ae.height)*(OT_EIS_VI_AE_ZONE_ROW)+vi_height-1)/vi_height);
    down_right = calc_1 < calc_2 ? calc_1 : calc_2;

    //AWB 32*32
    // >0
    calc_1 = (int)((rect_awb.x*OT_EIS_VI_AWB_ZONE_ORIG_COLUMN)/vi_width);
    up_left_awb = calc_1 > 0 ? calc_1 : 0;
    // min
    calc_1 = OT_EIS_VI_AWB_ZONE_ORIG_COLUMN-1;
    calc_2 = (int)(((rect_awb.x + rect_awb.width)*OT_EIS_VI_AWB_ZONE_ORIG_COLUMN+vi_width-1)/vi_width);
    up_right_awb = calc_1 < calc_2 ? calc_1 : calc_2;
    // >0
    calc_1 = (int)((rect_awb.y*OT_EIS_VI_AWB_ZONE_ORIG_ROW)/vi_height);
    down_left_awb = calc_1 > 0 ? calc_1 : 0;
    // min
    calc_1 = OT_EIS_VI_AWB_ZONE_ORIG_ROW-1;
    calc_2 = (int)(((rect_awb.y + rect_awb.height)*OT_EIS_VI_AWB_ZONE_ORIG_ROW+vi_height-1)/vi_height);
    down_right_awb = calc_1 < calc_2 ? calc_1 : calc_2;

    //AE crop
    //crop or not
    if((get_pipe_params()[index].dynamic_vproc.crop_ae_enable == OT_TRUE)&&((vproc_crop_info.enable == OT_TRUE) || (crop_mode == 1))) {
        if((get_pipe_params()[index].dynamic_vproc.crop_ae_enable_once == OT_TRUE)&&(g_dynamic_vproc_times==1)) {
            //maintain
            printf("crop_ae_enable_once:%d,g_dynamic_vproc_times:%d\n",get_pipe_params()[index].dynamic_vproc.crop_ae_enable_once,g_dynamic_vproc_times);
            //not change ae_weight crop
        } else {
            //crop
            ret = ot_eis_vi_stats_get_cfg(pipe_hdl, &isp_stats_cfg);
            check_scene_ret(ret);
            for (i = 0; i < OT_EIS_VI_AE_ZONE_ROW; i++) { //15
                for (j = 0; j < OT_EIS_VI_AE_ZONE_COLUMN; j++) {  //17
                    if((j>=up_left)&&(j<=up_right)&&(i>=down_left)&&(i<=down_right)) {
                        isp_stats_cfg.ae_cfg.weight[i][j] = get_pipe_params()[index].dynamic_vproc.ae_weight_inside;
                    } else {
                        isp_stats_cfg.ae_cfg.weight[i][j] = get_pipe_params()[index].dynamic_vproc.ae_weight_outside;
                    }
                }
            }
            ret = ot_eis_vi_stats_set_cfg(pipe_hdl, &isp_stats_cfg);
            check_scene_ret(ret);
        }
    }
    // restore static_weight
    // if(vproc_crop_info.enable != OT_TRUE)
    // {
    //         ret = ot_eis_vi_stats_get_cfg(pipe_hdl, &isp_stats_cfg);
    //         check_scene_ret(ret);
    //         for (i = 0; i < OT_EIS_VI_AE_ZONE_ROW; i++) { //15
    //             for (j = 0; j < OT_EIS_VI_AE_ZONE_COLUMN; j++) {  //17
    //             isp_stats_cfg.ae_cfg.weight[i][j] = get_pipe_params()[index].static_statistics.ae_weight[i][j];
    //             }
    //         }
    //         ret = ot_eis_vi_stats_set_cfg(pipe_hdl, &isp_stats_cfg);
    //         check_scene_ret(ret);
    // }

    //AWB crop
    //crop or not
    if((get_pipe_params()[index].dynamic_vproc.crop_awb_enable == OT_TRUE)&&((vproc_crop_info.enable == OT_TRUE) || (crop_mode == 1))) {
        if((get_pipe_params()[index].dynamic_vproc.crop_awb_enable_once == OT_TRUE)&&(g_dynamic_vproc_times==1)) {
            printf("crop_awb_enable_once:%d,g_dynamic_vproc_times:%d\n",get_pipe_params()[index].dynamic_vproc.crop_awb_enable_once,g_dynamic_vproc_times);
            //not change awb_weight crop
        } else {
            ret = ot_eis_vi_awb_get_attr(pipe_hdl, &wb_attr);
            check_scene_ret(ret);

            wb_attr.auto_attr.awb_zone_wt_en = OT_TRUE;

            for (i = 0; i < OT_EIS_VI_AWB_ZONE_ORIG_ROW; i++) { //32
                for (j = 0; j < OT_EIS_VI_AWB_ZONE_ORIG_COLUMN; j++) {  //32
                    if((j>=up_left_awb)&&(j<=up_right_awb)&&(i>=down_left_awb)&&(i<=down_right_awb)) {
                        wb_attr.auto_attr.zone_wt[i*OT_EIS_VI_AWB_ZONE_ORIG_COLUMN+j] = get_pipe_params()[index].dynamic_vproc.awb_weight_inside;
                    } else {
                        wb_attr.auto_attr.zone_wt[i*OT_EIS_VI_AWB_ZONE_ORIG_COLUMN+j] = get_pipe_params()[index].dynamic_vproc.awb_weight_outside;
                    }
                }
            }
            ret = ot_eis_vi_awb_set_attr(pipe_hdl, &wb_attr);
            check_scene_ret(ret);
        }
    }
    // restore default_weight,equal 8
    // if(vproc_crop_info.enable != OT_TRUE)
    // {
    //         ret = ot_eis_vi_awb_get_attr(pipe_hdl, &wb_attr);
    //         check_scene_ret(ret);

    //         wb_attr.auto_attr.awb_zone_wt_en = OT_TRUE;

    //         for (i = 0; i < OT_EIS_VI_AWB_ZONE_ORIG_ROW; i++) { //32
    //             for (j = 0; j < OT_EIS_VI_AWB_ZONE_ORIG_COLUMN; j++) {  //32
    //                 {
    //                     wb_attr.auto_attr.zone_wt[i*OT_EIS_VI_AWB_ZONE_ORIG_COLUMN+j] = 8;
    //                 }
    //             }
    //         }
    //         ret = ot_eis_vi_awb_set_attr(pipe_hdl, &wb_attr);
    //         check_scene_ret(ret);
    // }

    if(g_dynamic_vproc_times == 0) {
        g_dynamic_vproc_times = 1;
    } else {
        g_dynamic_vproc_times = 1;
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_ae(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.dynamic_ae != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_u32 exp_level;
    ot_s32 ret;
    ot_eis_vi_alg_state_info inner_state_info;
    ot_u32 actual_ratio;
    static ot_u32 last_ratio[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ... OT_EIS_VI_MAX_PIPE_NUM - 1] = 0x40};
    ot_eis_vi_exposure_attr exposure_attr;
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_query_alg_state_info(pipe_hdl, &inner_state_info);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    actual_ratio = inner_state_info.wdr_exp_ratio_actual[0];

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    if ((exposure != last_exposure) || (last_ratio[master_pipe_id] != actual_ratio)) {
        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_ae_get_exposure_attr(pipe_hdl, &exposure_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

        if (actual_ratio >= get_pipe_params()[index].dynamic_ae.wdr_ratio_threshold) {
            exposure_attr.advance_ae = get_pipe_params()[index].dynamic_ae.h_advance_ae;
        } else {
            exposure_attr.advance_ae = get_pipe_params()[index].dynamic_ae.l_advance_ae;
        }

        exp_level = scene_get_level_ltoh(exposure, get_pipe_params()[index].dynamic_ae.ae_exposure_cnt,
            get_pipe_params()[index].dynamic_ae.exp_ltoh_thresh);

        ot_scenecomm_expr_true_return(exp_level >= OT_SCENE_AE_EXPOSURE_MAX_COUNT, OT_FAILURE);
        if (exp_level == 0) {
            exposure_attr.auto_attr.compensation = get_pipe_params()[index].dynamic_ae.auto_compensation[exp_level];
            exposure_attr.auto_attr.max_hist_offset =
                get_pipe_params()[index].dynamic_ae.auto_max_hist_offset[exp_level];
        } else {
            exposure_attr.auto_attr.compensation =
                scene_interpulate(exposure, get_pipe_params()[index].dynamic_ae.exp_ltoh_thresh[exp_level - 1],
                    get_pipe_params()[index].dynamic_ae.auto_compensation[exp_level - 1],
                    get_pipe_params()[index].dynamic_ae.exp_ltoh_thresh[exp_level],
                    get_pipe_params()[index].dynamic_ae.auto_compensation[exp_level]);

            exposure_attr.auto_attr.max_hist_offset =
                scene_interpulate(exposure, get_pipe_params()[index].dynamic_ae.exp_ltoh_thresh[exp_level - 1],
                    get_pipe_params()[index].dynamic_ae.auto_max_hist_offset[exp_level - 1],
                    get_pipe_params()[index].dynamic_ae.exp_ltoh_thresh[exp_level],
                    get_pipe_params()[index].dynamic_ae.auto_max_hist_offset[exp_level]);
        }

        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_ae_set_exposure_attr(pipe_hdl, &exposure_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

        last_ratio[master_pipe_id] = actual_ratio;
    }
    return OT_SUCCESS;
}

static ot_s32 ot_scene_set_fswdr_part1(ot_eis_vi_wdr_attr *fswdr_attr, ot_u8 index, ot_u32 wdr_ratio, ot_u32  iso)
{
    ot_u32  ratio_count = get_pipe_params()[index].dynamic_fswdr.ratio_count;
    ot_u32 *ratio_level_thresh = get_pipe_params()[index].dynamic_fswdr.ratio_level;
    ot_u32  ratio_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)wdr_ratio, ratio_count, ratio_level_thresh);
    ot_scenecomm_expr_true_return(ratio_level >= ISP_AUTO_RATIO_FSWDR_NUM, OT_FAILURE);

    ot_u32  iso_count = get_pipe_params()[index].dynamic_fswdr.iso_count;
    ot_u32 *iso_level_thresh = get_pipe_params()[index].dynamic_fswdr.iso_ltoh_thresh;
    ot_u32  iso_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)iso, iso_count, iso_level_thresh);
    ot_scenecomm_expr_true_return(iso_level >= OT_EIS_SCENE_ISO_STRENGTH_NUM, OT_FAILURE);

    if (wdr_ratio <= get_pipe_params()[index].dynamic_fswdr.ratio_thr) {
        if ((iso_level == (iso_count - 1)) || (iso_level == 0)) {
            fswdr_attr->fusion_attr.fusion_blend_en =
                get_pipe_params()[index].dynamic_fswdr.fusion_blend_en[iso_level];
            fswdr_attr->fusion_attr.fusion_blend_wgt =
                get_pipe_params()[index].dynamic_fswdr.fusion_blend_wgt[iso_level];
        } else {
            fswdr_attr->fusion_attr.fusion_blend_en =
                get_pipe_params()[index].dynamic_fswdr.fusion_blend_en[iso_level - 1];
            fswdr_attr->fusion_attr.fusion_blend_wgt = scene_interpulate(iso,
                get_pipe_params()[index].dynamic_fswdr.iso_ltoh_thresh[iso_level - 1],
                get_pipe_params()[index].dynamic_fswdr.fusion_blend_wgt[iso_level - 1],
                get_pipe_params()[index].dynamic_fswdr.iso_ltoh_thresh[iso_level],
                get_pipe_params()[index].dynamic_fswdr.fusion_blend_wgt[iso_level]);
        }
    } else {
        if ((iso_level == (iso_count - 1)) || (iso_level == 0)) {
            fswdr_attr->fusion_attr.fusion_blend_en =
                get_pipe_params()[index].dynamic_fswdr.fusion_blend_en_h[iso_level];
            fswdr_attr->fusion_attr.fusion_blend_wgt =
                get_pipe_params()[index].dynamic_fswdr.fusion_blend_wgt_h[iso_level];
        } else {
            fswdr_attr->fusion_attr.fusion_blend_en =
                get_pipe_params()[index].dynamic_fswdr.fusion_blend_en_h[iso_level - 1];
            fswdr_attr->fusion_attr.fusion_blend_wgt = scene_interpulate(iso,
                get_pipe_params()[index].dynamic_fswdr.iso_ltoh_thresh[iso_level - 1],
                get_pipe_params()[index].dynamic_fswdr.fusion_blend_wgt_h[iso_level - 1],
                get_pipe_params()[index].dynamic_fswdr.iso_ltoh_thresh[iso_level],
                get_pipe_params()[index].dynamic_fswdr.fusion_blend_wgt_h[iso_level]);
        }
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_fswdr(ot_vi_pipe_handle pipe_hdl, ot_u32 iso, ot_u32 last_iso, ot_u8 index, ot_u32 wdr_ratio)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.dynamic_fswdr != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_vi_wdr_attr fswdr_attr;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_wdr_get_attr(pipe_hdl, &fswdr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ot_u32  ratio_count = get_pipe_params()[index].dynamic_fswdr.ratio_count;
    ot_u32 *ratio_level_thresh = get_pipe_params()[index].dynamic_fswdr.ratio_level;
    ot_u32  ratio_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)wdr_ratio, ratio_count, ratio_level_thresh);
    ot_scenecomm_expr_true_return(ratio_level >= ISP_AUTO_RATIO_FSWDR_NUM, OT_FAILURE);

    ot_u32  iso_count = get_pipe_params()[index].dynamic_fswdr.iso_count;
    ot_u32 *iso_level_thresh = get_pipe_params()[index].dynamic_fswdr.iso_ltoh_thresh;
    ot_u32  iso_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)iso, iso_count, iso_level_thresh);
    ot_scenecomm_expr_true_return(iso_level >= OT_EIS_SCENE_ISO_STRENGTH_NUM, OT_FAILURE);

    if ((ratio_level == (ratio_count - 1)) || (ratio_level == 0)) {
        fswdr_attr.wdr_merge_mode = get_pipe_params()[index].dynamic_fswdr.wdr_merge_mode[ratio_level];
        fswdr_attr.wdr_combine_attr.motion_comp = get_pipe_params()[index].dynamic_fswdr.motion_comp[ratio_level];
    } else {
        fswdr_attr.wdr_merge_mode = get_pipe_params()[index].dynamic_fswdr.wdr_merge_mode[ratio_level - 1];
        fswdr_attr.wdr_combine_attr.motion_comp = get_pipe_params()[index].dynamic_fswdr.motion_comp[ratio_level - 1];
    }
    ret = ot_scene_set_fswdr_part1(&fswdr_attr, index, wdr_ratio, iso);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_wdr_set_attr(pipe_hdl, &fswdr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_drc(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u32 wdr_ratio, ot_u32 iso)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.dynamic_drc != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_u32 iso_level;
    ot_u32 ratio_level = 0;
    ot_u32 interval = get_pipe_params()[index].dynamic_drc.interval;
    ot_u32 ratio_count = get_pipe_params()[index].dynamic_drc.ratio_count;
    ot_u32 *ratio_level_thresh = get_pipe_params()[index].dynamic_drc.ratio_level;
    ot_u32 iso_count = get_pipe_params()[index].dynamic_drc.iso_count;
    ot_u32 *iso_level_thresh = get_pipe_params()[index].dynamic_drc.iso_level;

    ot_eis_vi_drc_attr isp_drc_attr;
    ot_eis_vi_public_attr pub_attr;
    ot_eis_vi_alg_state_info inner_state_info;

    check_scene_return_if_pause(pipe_hdl);
    ot_s32 ret = ot_eis_vi_public_get_attr(pipe_hdl, &pub_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    if (pub_attr.wdr_mode == OT_EIS_WDR_MODE_NONE) {
        return OT_SUCCESS;
    }

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_drc_get_attr(pipe_hdl, &isp_drc_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_query_alg_state_info(pipe_hdl, &inner_state_info);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    if (pub_attr.wdr_mode == OT_EIS_WDR_MODE_2To1_FRAME) {
        ratio_level =
            scene_get_level_ltoh_u32(inner_state_info.wdr_exp_ratio_actual[0], ratio_count, ratio_level_thresh);
    } else {
        ratio_level = scene_get_level_ltoh_u32(wdr_ratio, ratio_count, ratio_level_thresh);
    }
    iso_level = scene_get_level_ltoh_u32(iso, iso_count, iso_level_thresh);
    ot_scenecomm_expr_true_return(ratio_level >= OT_EIS_SCENE_DRC_RATIO_MAX_COUNT, OT_FAILURE);
    ot_scenecomm_expr_true_return(iso_level >= OT_EIS_SCENE_DRC_ISO_MAX_COUNT, OT_FAILURE);

    set_isp_attr_param param = { index, iso_level, iso_count, ratio_level, ratio_count, interval, wdr_ratio, iso };
    scene_set_isp_attr(pipe_hdl,param, ratio_level_thresh, iso_level_thresh, &isp_drc_attr);

    ///* right shift 3 */
    //ret = scene_set_tone_mapping_value(pipe_hdl, index, (interval >> 3), &inner_state_info, &isp_drc_attr);
    //ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_drc_set_attr(pipe_hdl, &isp_drc_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

static ot_s32 scene_get_dynamic_dehaze_str(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u8 exp_level,
    ot_u64 *manual_str_l, ot_u64 *manual_str_r)
{
    ot_s32 ret = OT_SUCCESS;
    ot_eis_vi_alg_state_info state_info;
    ot_u8 is_ratio_higher = 0;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_query_alg_state_info(pipe_hdl, &state_info);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    is_ratio_higher =
        (state_info.wdr_exp_ratio_actual[0] > get_pipe_params()[index].dynamic_dehaze.wdr_ratio_threshold) ? 1 : 0;
    if (is_ratio_higher == 1) {
        *manual_str_l = (ot_u64)get_pipe_params()[index].dynamic_dehaze.manual_strengther[exp_level - 1];
        *manual_str_r = (ot_u64)get_pipe_params()[index].dynamic_dehaze.manual_strengther[exp_level];
    } else {
        *manual_str_l = (ot_u64)get_pipe_params()[index].dynamic_dehaze.manual_strength[exp_level - 1];
        *manual_str_r = (ot_u64)get_pipe_params()[index].dynamic_dehaze.manual_strength[exp_level];
    }

    return ret;
}

static ot_s32 g_bl_level[16] = {8000,8000,8000,8000,8000,8000,8000,8000,8000,8000,8000,8000,8000,8000,8000,8000}; //default, base 10000

ot_s32 ot_scene_set_dynamic_back_light(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    //converge when iso fix
    if ((get_pipe_params()[index].module_state.dynamic_back_light != OT_TRUE) ) {
        return OT_SUCCESS;
    }

    ot_bool info_print = get_pipe_params()[index].dynamic_back_light.info_print;

    //enable
    if(get_pipe_params()[index].dynamic_back_light.enable != OT_TRUE)
    {
        if(info_print)
        {
            printf("dynamic_back_light disable!\n");
        }
        return OT_SUCCESS;
    }

    //run by mode
    ot_s32 ret;
    // ot_s32 i;

    ot_s32 bl_calc_array = 0;
    ot_s32 bl_calc_result;
    ot_u32 pwm_value;
    // sample_light_sensor light_sensor_attr;
    ot_u32 light_sensor_value = 0;
    ot_u32 light_level;
    ot_u32 iso_level;

    ot_u32 base = get_pipe_params()[index].dynamic_back_light.backlight_base;
    ot_u32 array_cnt = get_pipe_params()[index].dynamic_back_light.array_cnt;
    ot_u32 bl_min = get_pipe_params()[index].dynamic_back_light.backlight_min;
    ot_u32 bl_max = get_pipe_params()[index].dynamic_back_light.backlight_max;
    ot_u32 bl_tol = get_pipe_params()[index].dynamic_back_light.backlight_tolerance;

    ot_u32 bl_conv_rt_inc = get_pipe_params()[index].dynamic_back_light.backlight_conv_ratio_inc;
    ot_u32 bl_conv_rt_dec = get_pipe_params()[index].dynamic_back_light.backlight_conv_ratio_dec;

    ot_u32 * light_array = get_pipe_params()[index].dynamic_back_light.light_array;
    ot_u32 * bl_array_light = get_pipe_params()[index].dynamic_back_light.backlight_array_light;
    ot_u32 * iso_array = get_pipe_params()[index].dynamic_back_light.iso_array;
    ot_u32 * bl_array_iso = get_pipe_params()[index].dynamic_back_light.backlight_array_iso;

    switch(get_pipe_params()[index].dynamic_back_light.backlight_mode)
    {
        case 0: //fix
            if(info_print)
            {
                printf("dynamic_back_light fix mode!\n");
            }
            return OT_SUCCESS;
            //break;

        case 1: //ctrl by light sensor, light sensor should be start fisrt
            light_sensor_value = sample_comm_lightsensor_get_lux(index);

            //get array value
            light_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)light_sensor_value, array_cnt, light_array);
            ot_scenecomm_expr_true_return(light_level >= 16, OT_FAILURE);

            if ((light_level == (array_cnt - 1)) || (light_level == 0)) {
                bl_calc_array = bl_array_light[light_level];
            } else {
                bl_calc_array = scene_interpulate(light_level,light_array[light_level - 1],bl_array_light[light_level - 1],light_array[light_level],bl_array_light[light_level]);
            }
            break;

        case 2: //ctrl by iso
            //get array value
            iso_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)iso, array_cnt, iso_array);
            ot_scenecomm_expr_true_return(iso_level >= 16, OT_FAILURE);

            if ((iso_level == (array_cnt - 1)) || (iso_level == 0)) {
                bl_calc_array = bl_array_iso[iso_level];
            } else {
                bl_calc_array = scene_interpulate(iso,iso_array[iso_level - 1],bl_array_iso[iso_level - 1],iso_array[iso_level],bl_array_iso[iso_level]);
            }
        break;
    }

    //valid range, clip
    bl_calc_result = (bl_calc_array > bl_max) ? bl_max : bl_calc_array;
    bl_calc_result = (bl_calc_array < bl_min) ? bl_min : bl_calc_array;

    //calc prec
    bl_calc_result = bl_calc_result * 10000 / base;

    //increase
    if(bl_calc_result > g_bl_level[index])
    {
        if ((bl_calc_result - g_bl_level[index]) > bl_tol)
        {
            bl_calc_result = g_bl_level[index] + (bl_calc_result - g_bl_level[index]) * bl_conv_rt_inc / 100;
        }
    }
    //decrease
    else if(bl_calc_result < g_bl_level[index])
    {
        if((g_bl_level[index] - bl_calc_result) > bl_tol)
        {
            bl_calc_result = g_bl_level[index] - (g_bl_level[index] - bl_calc_result) * bl_conv_rt_dec / 100;
        }
    }
    //equal
    else
    {
        if(info_print)
        {
            printf("back_light not change, backlight_level:%d\n",g_bl_level[index]);
        }
        return OT_SUCCESS;
    }
    pwm_value = bl_calc_result * base /10000;      //unit

    //ctrl back_light
    //pipe dev id
    // ot_u32 dev_num = get_pipe_params()[index].dynamic_back_light.dev_num;     //tbd
    ot_u32 * dev_id = get_pipe_params()[index].dynamic_back_light.dev_id;

    ot_u32 pwm_id = dev_id[index];

    ret = sample_comm_start_pwm(pwm_id, pwm_value);
    if (ret != OT_SUCCESS) {
        sample_print("pwm %hhu enable failed\n", pwm_id);
        return OT_FAILURE;
    }
    g_bl_level[index] = bl_calc_result;      //restore calc result,not effective result,prec

    if(info_print)
    {
        //light sensor mode
        if(get_pipe_params()[index].dynamic_back_light.backlight_mode == 1)
        {
            printf("index:%d,\tbacklight_mode:%d,\tlight_sensor_value:%d,\tbl_calc_array:%d,\tbl_calc_result:%d,\tpwm_value:%d\n",index,get_pipe_params()[index].dynamic_back_light.backlight_mode,light_sensor_value,bl_calc_array,bl_calc_result,pwm_value);
        }
        //iso mode
        if(get_pipe_params()[index].dynamic_back_light.backlight_mode == 2)
        {
            printf("index:%d,\tbacklight_mode:%d,\tiso:%lld,\tbl_calc_array:%d,\tbl_calc_result:%d,\tpwm_value:%d\n",index,get_pipe_params()[index].dynamic_back_light.backlight_mode,iso,bl_calc_array,bl_calc_result,pwm_value);
        }
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_static_back_light(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.static_back_light != OT_TRUE) {
        return OT_SUCCESS;
    }
    ot_s32 ret;

    // ot_u32 dev_num = get_pipe_params()[index].static_back_light.dev_num;
    ot_u32 * dev_id = get_pipe_params()[index].static_back_light.dev_id;
    ot_u32 * backlight_array = get_pipe_params()[index].static_back_light.backlight_array;

    //dev pwm_value
    ot_u32 pwm_id = dev_id[index];
    ot_u32 pwm_value = backlight_array[index];      //unit

    // printf("pwm_id:%d,pwm_value:%d\n",pwm_id,pwm_value);
    ret = sample_comm_start_pwm(pwm_id, pwm_value);
    if (ret != OT_SUCCESS) {
        sample_print("pwm %hhu enable failed\n", pwm_id);
        return OT_FAILURE;
    }

    //calc prec
    g_bl_level[index] = pwm_value * 10000 / get_pipe_params()[index].static_back_light.backlight_base;

    return OT_SUCCESS;
}

static ot_s32 conv_ratio_1 = 2;
static ot_s32 conv_ratio_2 = 10;

ot_s32 ot_scene_set_dynamic_dehaze(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_dehaze != OT_TRUE)) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_vi_dehaze_attr dehaze_attr;
    ot_u32 exp_level;
    ot_u64 man_str_l = 0;
    ot_u64 man_str_r = 0;
    ot_eis_vi_alg_state_info state_info;
    static ot_u32 last_ratio[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ... OT_EIS_VI_MAX_PIPE_NUM - 1] = 0x40};
    ot_u32 actual_ratio;
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_u32 haze_level_index = 0;
    ot_u32 lut_luma_index = 0;
    ot_eis_scene_haze_level_info haze_info;
    ot_u64 mg_aver = 0;
    ot_eis_vi_mg_stats mg_stat;
    ot_u16 dehaze_lut_value[OT_EIS_VI_DEHAZE_LUT_SIZE] = {0};

    if(get_pipe_params()[index].dynamic_dehaze.dehzae_level_enable == 0) {
        ret = ot_eis_vi_query_alg_state_info(pipe_hdl, &state_info);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

        ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
        master_pipe_id = pipe_id[0];
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

        actual_ratio = state_info.wdr_exp_ratio_actual[0];
        if ((exposure != last_exposure) || (last_ratio[master_pipe_id] != actual_ratio)) {
            check_scene_return_if_pause(pipe_hdl);
            ret  = ot_eis_vi_dehaze_get_attr(pipe_hdl, &dehaze_attr);
            ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

            exp_level = scene_get_level_ltoh(exposure, get_pipe_params()[index].dynamic_dehaze.exp_thresh_cnt,
                get_pipe_params()[index].dynamic_dehaze.exp_thresh_ltoh);
            ot_scenecomm_expr_true_return(exp_level >= OT_SCENE_DEHAZE_EXPOSURE_MAX_COUNT, OT_FAILURE);

            ret = scene_get_dynamic_dehaze_str(pipe_hdl, index, exp_level, &man_str_l, &man_str_r);
            ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

            if (dehaze_attr.op_type == 0) {
                if ((exp_level == 0) || (exp_level == (get_pipe_params()[index].dynamic_dehaze.exp_thresh_cnt - 1))) {
                    dehaze_attr.auto_attr.strength = man_str_r;
                } else {
                    dehaze_attr.auto_attr.strength =
                        scene_interpulate(exposure, get_pipe_params()[index].dynamic_dehaze.exp_thresh_ltoh[exp_level - 1],
                        man_str_l, get_pipe_params()[index].dynamic_dehaze.exp_thresh_ltoh[exp_level], man_str_r);
                }
            } else if (dehaze_attr.op_type == 1) {
                if ((exp_level == 0) || (exp_level == (get_pipe_params()[index].dynamic_dehaze.exp_thresh_cnt - 1))) {
                    dehaze_attr.manual_attr.strength = man_str_r;
                } else {
                    dehaze_attr.manual_attr.strength =
                        scene_interpulate(exposure, get_pipe_params()[index].dynamic_dehaze.exp_thresh_ltoh[exp_level - 1],
                        man_str_l, get_pipe_params()[index].dynamic_dehaze.exp_thresh_ltoh[exp_level], man_str_r);
                }
            }

            check_scene_return_if_pause(pipe_hdl);
            ret = ot_eis_vi_dehaze_set_attr(pipe_hdl, &dehaze_attr);
            ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

            last_ratio[master_pipe_id] = actual_ratio;
        }
    } else if(get_pipe_params()[index].dynamic_dehaze.dehzae_level_enable == 1) {
        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_dehaze_get_attr(pipe_hdl, &dehaze_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

        check_scene_return_if_pause(pipe_hdl);

        ret = ot_eis_scene_haze_level_get_info(pipe_hdl, &haze_info);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

        haze_level_index = scene_get_level_ltoh(
            haze_info.haze_level, get_pipe_params()[index].dynamic_dehaze.dehaze_level_cnt,
            get_pipe_params()[index].dynamic_dehaze.dehaze_level_threshold);
        ot_scenecomm_expr_true_return(
            haze_level_index >= OT_SCENE_DEHAZE_EXPOSURE_MAX_COUNT, OT_FAILURE);

        if (haze_level_index == 0) {
            dehaze_attr.manual_attr.strength =
                get_pipe_params()[index].dynamic_dehaze.dehaze_level_strength[0];
        } else {
            dehaze_attr.manual_attr.strength = scene_interpulate(
                haze_info.haze_level,
                get_pipe_params()[index]
                    .dynamic_dehaze.dehaze_level_threshold[haze_level_index - 1],
                get_pipe_params()[index]
                    .dynamic_dehaze.dehaze_level_strength[haze_level_index - 1],
                get_pipe_params()[index]
                    .dynamic_dehaze.dehaze_level_threshold[haze_level_index],
                get_pipe_params()[index]
                    .dynamic_dehaze.dehaze_level_strength[haze_level_index]);

            if(dehaze_attr.manual_attr.strength > g_dehaze_str)
            {
                dehaze_attr.manual_attr.strength = g_dehaze_str + (dehaze_attr.manual_attr.strength - g_dehaze_str) * conv_ratio_1 / conv_ratio_2;
                g_dehaze_str = dehaze_attr.manual_attr.strength;
            }
            else if(dehaze_attr.manual_attr.strength < g_dehaze_str)
            {
                dehaze_attr.manual_attr.strength = dehaze_attr.manual_attr.strength + (g_dehaze_str - dehaze_attr.manual_attr.strength) * conv_ratio_1 / conv_ratio_2;
                g_dehaze_str = dehaze_attr.manual_attr.strength;
            }
        }

        if(get_pipe_params()[index].dynamic_dehaze.dehaze_lut_tune_en == 1)
        {
            ret = ot_eis_vi_mg_get_stats(pipe_hdl, &mg_stat);
            if (ret != OT_SUCCESS) {
                printf("get mg stats failed with 0x%x\n", ret);
                return OT_ERR_EIS_SCENE_EINTER;
            }

            for(int i = 0;i < OT_EIS_VI_BAYER_CHN_NUM; i++) {
                for(int j = 0; j < OT_EIS_VI_MG_ZONE_COLUMN; j++) {
                    for(int k = 0; k < OT_EIS_VI_MG_ZONE_ROW; k++) {
                        mg_aver += mg_stat.zone_avg[k][j][i];
                    }
                }
            }

            mg_aver /= (OT_EIS_VI_BAYER_CHN_NUM * OT_EIS_VI_MG_ZONE_COLUMN * OT_EIS_VI_MG_ZONE_ROW);

            lut_luma_index = scene_get_level_ltoh(
                mg_aver, get_pipe_params()[index].dynamic_dehaze.dehaze_lut_luma_cnt,
                get_pipe_params()[index].dynamic_dehaze.dehaze_lut_luma_thr);

            for (int j = 0; j < OT_EIS_VI_DEHAZE_LUT_SIZE; j++) {
                switch (lut_luma_index)
                {
                    case 0:
                        dehaze_lut_value[j] = get_pipe_params()[index].dynamic_dehaze.dehaze_lut_1[j];
                        break;
                    case 1:
                        dehaze_lut_value[j] = scene_interpulate(mg_aver,get_pipe_params()[index].dynamic_dehaze.dehaze_lut_luma_thr[lut_luma_index - 1],get_pipe_params()[index].dynamic_dehaze.dehaze_lut_1[j],get_pipe_params()[index].dynamic_dehaze.dehaze_lut_luma_thr[lut_luma_index],get_pipe_params()[index].dynamic_dehaze.dehaze_lut_2[j]);
                        break;
                    case 2:
                        dehaze_lut_value[j] = scene_interpulate(mg_aver,get_pipe_params()[index].dynamic_dehaze.dehaze_lut_luma_thr[lut_luma_index - 1],get_pipe_params()[index].dynamic_dehaze.dehaze_lut_2[j],get_pipe_params()[index].dynamic_dehaze.dehaze_lut_luma_thr[lut_luma_index],get_pipe_params()[index].dynamic_dehaze.dehaze_lut_3[j]);
                        break;
                    case 3:
                        dehaze_lut_value[j] = scene_interpulate(mg_aver,get_pipe_params()[index].dynamic_dehaze.dehaze_lut_luma_thr[lut_luma_index - 1],get_pipe_params()[index].dynamic_dehaze.dehaze_lut_3[j],get_pipe_params()[index].dynamic_dehaze.dehaze_lut_luma_thr[lut_luma_index],get_pipe_params()[index].dynamic_dehaze.dehaze_lut_4[j]);
                        break;
                    default:
                            scene_loge("scene_auto dynamic_dehaze lut_luma_index invalid\n");
                        break;
                }
            }
            for (int j = 0; j < OT_EIS_VI_DEHAZE_LUT_SIZE; j++) {
                //p1 base,step = ((1+p4)/(p3)) * abs(p1 -p2), 2/15
                dehaze_attr.dehaze_lut[j] = scene_time_filter(dehaze_attr.dehaze_lut[j],
                    dehaze_lut_value[j], 15, 1);
                }
        }

        if (get_pipe_params()[index].dynamic_dehaze.dehaze_level_print) {
            printf("haze_level:%d\n", haze_info.haze_level);
            printf("haze_level_index:%d\n", haze_level_index);
            printf("mg_aver:%lld\n", mg_aver);
            printf("lut_luma_index:%d\n", lut_luma_index);
            printf("dehaze_attr.manual_attr.strength:%d\n", dehaze_attr.manual_attr.strength);
        }

        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_dehaze_set_attr(pipe_hdl, &dehaze_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    } else {
        scene_loge("unvalid dehzae_level_enable\n");
    }

    return OT_SUCCESS;
}

static ot_s32 ot_scene_set_dynamic_nr_part1(ot_scene_nr_para *nr_para, ot_u8 index, ot_u32 wdr_ratio)
{
    ot_u32 i, ratio_index;
    ot_u32 ratio_count = get_pipe_params()[index].dynamic_nr.ratio_count;
    ot_u32 *level = get_pipe_params()[index].dynamic_nr.ratio_level;

    ratio_index = scene_get_level_ltoh_u32(wdr_ratio, ratio_count, level);
    ot_scenecomm_expr_true_return(ratio_index >= OT_EIS_SCENE_DRC_RATIO_MAX_COUNT, OT_FAILURE);

    if ((ratio_index == 0) || (ratio_index == (ratio_count - 1))) {
        for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
            nr_para->md_fusion_incr[i] = get_pipe_params()[index].dynamic_nr.md_fusion_frame_str_incr[i][ratio_index];
            nr_para->sfm0_fusion_incr[i] =
                get_pipe_params()[index].dynamic_nr.snr_sfm0_fusion_frame_str_incr[i][ratio_index];

            nr_para->md_wdr_incr[i] = get_pipe_params()[index].dynamic_nr.md_wdr_frame_str_incr[i][ratio_index];
            nr_para->sfm0_wdr_incr[i] = get_pipe_params()[index].dynamic_nr.snr_sfm0_wdr_frame_str_incr[i][ratio_index];
            nr_para->snr_fusion_sfm6_sfm7_incr[i] =
                get_pipe_params()[index].dynamic_nr.snr_fusion_sfm6_sfm7_frame_incr[i][ratio_index];
            nr_para->snr_wdr_sfm6_sfm7_incr[i] =
                get_pipe_params()[index].dynamic_nr.snr_wdr_sfm6_sfm7_frame_incr[i][ratio_index];
        }
    } else {
        for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
            nr_para->md_fusion_incr[i] = scene_interpulate(wdr_ratio, level[ratio_index - 1],
                get_pipe_params()[index].dynamic_nr.md_fusion_frame_str_incr[i][ratio_index - 1],
                level[ratio_index], get_pipe_params()[index].dynamic_nr.md_fusion_frame_str_incr[i][ratio_index]);

            nr_para->sfm0_fusion_incr[i] = scene_interpulate(wdr_ratio, level[ratio_index - 1],
                get_pipe_params()[index].dynamic_nr.snr_sfm0_fusion_frame_str_incr[i][ratio_index - 1],
                level[ratio_index], get_pipe_params()[index].dynamic_nr.snr_sfm0_fusion_frame_str_incr[i][ratio_index]);

            nr_para->md_wdr_incr[i] = scene_interpulate(wdr_ratio, level[ratio_index - 1],
                get_pipe_params()[index].dynamic_nr.md_wdr_frame_str_incr[i][ratio_index - 1],
                level[ratio_index], get_pipe_params()[index].dynamic_nr.md_wdr_frame_str_incr[i][ratio_index]);

            nr_para->sfm0_wdr_incr[i] = scene_interpulate(wdr_ratio, level[ratio_index - 1],
                get_pipe_params()[index].dynamic_nr.snr_sfm0_wdr_frame_str_incr[i][ratio_index - 1],
                level[ratio_index], get_pipe_params()[index].dynamic_nr.snr_sfm0_wdr_frame_str_incr[i][ratio_index]);
            nr_para->snr_fusion_sfm6_sfm7_incr[i] = scene_interpulate(wdr_ratio, level[ratio_index - 1],
                get_pipe_params()[index].dynamic_nr.snr_fusion_sfm6_sfm7_frame_incr[i][ratio_index - 1],
                level[ratio_index],
                get_pipe_params()[index].dynamic_nr.snr_fusion_sfm6_sfm7_frame_incr[i][ratio_index]);
            nr_para->snr_wdr_sfm6_sfm7_incr[i] = scene_interpulate(wdr_ratio, level[ratio_index - 1],
                get_pipe_params()[index].dynamic_nr.snr_wdr_sfm6_sfm7_frame_incr[i][ratio_index - 1],
                level[ratio_index],
                get_pipe_params()[index].dynamic_nr.snr_wdr_sfm6_sfm7_frame_incr[i][ratio_index]);
        }
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_nr(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u32 wdr_ratio, ot_u32 iso)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.dynamic_nr != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_u32 ratio_index;
    ot_u32 ratio_count = get_pipe_params()[index].dynamic_nr.ratio_count;
    ot_u32 *level = get_pipe_params()[index].dynamic_nr.ratio_level;
    ot_scene_nr_para nr_para = {{0}};
    ot_eis_vi_nr_attr nr_attr;

    ratio_index = scene_get_level_ltoh_u32(wdr_ratio, ratio_count, level);
    ot_scenecomm_expr_true_return(ratio_index >= OT_EIS_SCENE_DRC_RATIO_MAX_COUNT, OT_FAILURE);

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_nr_get_attr(pipe_hdl, &nr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_dynamic_nr_part1(&nr_para, index, wdr_ratio);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = scene_set_nr_attr_para(index, iso, &nr_attr, &nr_para);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = scene_set_nr_wdr_ratio_para(pipe_hdl, index, wdr_ratio, ratio_index, &nr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_nr_set_attr(pipe_hdl, &nr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

static ot_s32 ot_scene_set_dynamic_dpc_part1(ot_eis_vi_dpc_dynamic_cor_attr *dp_dynamic_attr, ot_u8 index, ot_u64 iso)
{
    ot_s32 i;
    ot_u32  iso_count = get_pipe_params()[index].dynamic_dpc.iso_count;
    ot_u32 *iso_level_thresh = get_pipe_params()[index].dynamic_dpc.iso_level;
    ot_u32 iso_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)iso, iso_count, iso_level_thresh);
    ot_scenecomm_expr_true_return(iso_level >= ISP_AUTO_ISO_DPC_NUM, OT_FAILURE);

    if ((iso_level == (iso_count - 1)) || (iso_level == 0)) {
        for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
            dp_dynamic_attr->frame_dynamic[i].sup_twinkle_en =
                get_pipe_params()[index].dynamic_dpc.sup_twinkle_en[i][iso_level];
            dp_dynamic_attr->frame_dynamic[i].soft_thr =
                (ot_s8)get_pipe_params()[index].dynamic_dpc.soft_thr[i][iso_level];
            dp_dynamic_attr->frame_dynamic[i].soft_slope =
                get_pipe_params()[index].dynamic_dpc.soft_slope[i][iso_level];
            dp_dynamic_attr->frame_dynamic[i].bright_strength =
                get_pipe_params()[index].dynamic_dpc.bright_strength[i][iso_level];
            dp_dynamic_attr->frame_dynamic[i].dark_strength =
                get_pipe_params()[index].dynamic_dpc.dark_strength[i][iso_level];
        }
    } else {
        ot_u32 mid_iso = (ot_u32)iso;
        ot_u32 left_iso = iso_level_thresh[iso_level - 1];
        ot_u32 right_iso = iso_level_thresh[iso_level];
        for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
            dp_dynamic_attr->frame_dynamic[i].sup_twinkle_en =
                get_pipe_params()[index].dynamic_dpc.sup_twinkle_en[i][iso_level - 1];
            dp_dynamic_attr->frame_dynamic[i].soft_thr = scene_interpulate(mid_iso, left_iso,
                get_pipe_params()[index].dynamic_dpc.soft_thr[i][iso_level - 1], right_iso,
                get_pipe_params()[index].dynamic_dpc.soft_thr[i][iso_level]);
            dp_dynamic_attr->frame_dynamic[i].soft_slope = scene_interpulate(mid_iso, left_iso,
                get_pipe_params()[index].dynamic_dpc.soft_slope[i][iso_level - 1], right_iso,
                get_pipe_params()[index].dynamic_dpc.soft_slope[i][iso_level]);
            dp_dynamic_attr->frame_dynamic[i].bright_strength = scene_interpulate(mid_iso, left_iso,
                get_pipe_params()[index].dynamic_dpc.bright_strength[i][iso_level - 1], right_iso,
                get_pipe_params()[index].dynamic_dpc.bright_strength[i][iso_level]);
            dp_dynamic_attr->frame_dynamic[i].dark_strength = scene_interpulate(mid_iso, left_iso,
                get_pipe_params()[index].dynamic_dpc.dark_strength[i][iso_level - 1], right_iso,
                get_pipe_params()[index].dynamic_dpc.dark_strength[i][iso_level]);
        }
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_dpc(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_dpc != OT_TRUE) || (last_iso == iso)) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_vi_dpc_dynamic_cor_attr dp_dynamic_attr;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_dpc_get_dynamic_cor_attr(pipe_hdl, &dp_dynamic_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_dynamic_dpc_part1(&dp_dynamic_attr, index, iso);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_dpc_set_dynamic_cor_attr(pipe_hdl, &dp_dynamic_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_awb(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index)
{

    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_awb != OT_TRUE) || (iso == last_iso)) {
        return OT_SUCCESS;
    }

    ot_u32 iso_level;
    ot_u32 iso_count = get_pipe_params()[index].dynamic_awb.iso_count;
    ot_u32 *piso_level_thresh = get_pipe_params()[index].dynamic_awb.iso_level;

    ot_s32 ret;
    ot_eis_vi_awb_attr wb_attr;
    ot_eis_vi_awb_ex_attr awb_attr_ex;
    ot_eis_vi_awb_anti_blue_interfere_attr anti_blue_interfere_attr;

    ret = ot_eis_vi_awb_get_attr(pipe_hdl, &wb_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_awb_get_ex_attr(pipe_hdl, &awb_attr_ex);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret =ot_eis_vi_awb_get_anti_blue_attr(pipe_hdl,&anti_blue_interfere_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    iso_level = scene_get_level_ltoh_u32((ot_u32)iso, iso_count, piso_level_thresh);
    ot_scenecomm_expr_true_return(iso_level >= OT_EIS_SCENE_ISO_STRENGTH_NUM, OT_FAILURE);
    if ((iso_level == 0) || (iso_level == (iso_count - 1))) {
        wb_attr.auto_attr.low_color_temp = get_pipe_params()[index].dynamic_awb.auto_low_color_temp[iso_level];
        wb_attr.auto_attr.high_color_temp = get_pipe_params()[index].dynamic_awb.auto_high_color_temp[iso_level];
        awb_attr_ex.in_or_out.op_type = get_pipe_params()[index].dynamic_awb.in_out_op_mode[iso_level];
        awb_attr_ex.in_or_out.scene_status = get_pipe_params()[index].dynamic_awb.manual_in_out_status[iso_level];
        awb_attr_ex.in_or_out.low_start = get_pipe_params()[index].dynamic_awb.low_start[iso_level];
        awb_attr_ex.in_or_out.low_stop = get_pipe_params()[index].dynamic_awb.low_stop[iso_level];
        awb_attr_ex.in_or_out.high_start = get_pipe_params()[index].dynamic_awb.high_start[iso_level];
        awb_attr_ex.in_or_out.high_stop = get_pipe_params()[index].dynamic_awb.high_stop[iso_level];

        wb_attr.auto_attr.shift_limit_en = get_pipe_params()[index].dynamic_awb.focus_en[iso_level];
        awb_attr_ex.zone_radius = get_pipe_params()[index].dynamic_awb.zone_radius[iso_level];
        wb_attr.auto_attr.shift_limit = get_pipe_params()[index].dynamic_awb.shift_limit[iso_level];
        awb_attr_ex.in_or_out.out_shift_limit = get_pipe_params()[index].dynamic_awb.out_shift_limit[iso_level];

        anti_blue_interfere_attr.blue_discard_ct_thresh_indoor = get_pipe_params()[index].dynamic_awb.bd_ct_th_indoor[iso_level];
        anti_blue_interfere_attr.blue_discard_ct_thresh_outdoor = get_pipe_params()[index].dynamic_awb.bd_ct_th_outdoor[iso_level];
        anti_blue_interfere_attr.blue_moment_ctn_ct_thresh = get_pipe_params()[index].dynamic_awb.bm_ctn_ct_th[iso_level];
        anti_blue_interfere_attr.blue_moment_auto_outdoor_num_thresh = get_pipe_params()[index].dynamic_awb.bm_au_out_num[iso_level];
        anti_blue_interfere_attr.blue_moment_sft = get_pipe_params()[index].dynamic_awb.bm_sft[iso_level];
    } else {
        wb_attr.auto_attr.low_color_temp = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.auto_low_color_temp[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.auto_low_color_temp[iso_level]);

        wb_attr.auto_attr.high_color_temp = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.auto_high_color_temp[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.auto_high_color_temp[iso_level]);

        awb_attr_ex.in_or_out.op_type = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.in_out_op_mode[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.in_out_op_mode[iso_level]);

        awb_attr_ex.in_or_out.scene_status = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.manual_in_out_status[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.manual_in_out_status[iso_level]);

        awb_attr_ex.in_or_out.low_start = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.low_start[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.low_start[iso_level]);

        awb_attr_ex.in_or_out.low_stop = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.low_stop[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.low_stop[iso_level]);

        awb_attr_ex.in_or_out.high_start = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.high_start[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.high_start[iso_level]);

        awb_attr_ex.in_or_out.high_stop = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.high_stop[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.high_stop[iso_level]);

        wb_attr.auto_attr.shift_limit_en = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.focus_en[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.focus_en[iso_level]);

        awb_attr_ex.zone_radius = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.zone_radius[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.zone_radius[iso_level]);

        wb_attr.auto_attr.shift_limit = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.shift_limit[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.shift_limit[iso_level]);

        awb_attr_ex.in_or_out.out_shift_limit = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.out_shift_limit[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.out_shift_limit[iso_level]);

        anti_blue_interfere_attr.blue_discard_ct_thresh_indoor = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.bd_ct_th_indoor[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.bd_ct_th_indoor[iso_level]);

        anti_blue_interfere_attr.blue_discard_ct_thresh_outdoor = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.bd_ct_th_outdoor[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.bd_ct_th_outdoor[iso_level]);

        anti_blue_interfere_attr.blue_moment_ctn_ct_thresh = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.bm_ctn_ct_th[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.bm_ctn_ct_th[iso_level]);

        anti_blue_interfere_attr.blue_moment_auto_outdoor_num_thresh = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.bm_au_out_num[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.bm_au_out_num[iso_level]);

        anti_blue_interfere_attr.blue_moment_sft = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_awb.bm_sft[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_awb.bm_sft[iso_level]);
    }

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_awb_set_attr(pipe_hdl, &wb_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_awb_set_ex_attr(pipe_hdl, &awb_attr_ex);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    ret = ot_eis_vi_awb_set_anti_blue_attr(pipe_hdl, &anti_blue_interfere_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}


ot_s32 ot_scene_set_dynamic_shading(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_shading != OT_TRUE) || (last_exposure == exposure)) {
        return OT_SUCCESS;
    }

    ot_u32 exp_level;
    ot_s32 ret;
    ot_eis_vi_shading_attr shading_attr;

    if (exposure != last_exposure) {
        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_mlsc_get_attr(pipe_hdl, &shading_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

        exp_level = scene_get_level_ltoh(exposure, get_pipe_params()[index].dynamic_shading.exp_thresh_cnt,
            get_pipe_params()[index].dynamic_shading.exp_thresh_ltoh);
        ot_scenecomm_expr_true_return(exp_level >= OT_SCENE_SHADING_EXPOSURE_MAX_COUNT, OT_FAILURE);
        if ((exp_level == 0) || (exp_level == (get_pipe_params()[index].dynamic_shading.exp_thresh_cnt - 1))) {
            shading_attr.mesh_strength = get_pipe_params()[index].dynamic_shading.mesh_strength[exp_level];
        } else {
            shading_attr.mesh_strength =
                scene_interpulate(exposure, get_pipe_params()[index].dynamic_shading.exp_thresh_ltoh[exp_level - 1],
                    get_pipe_params()[index].dynamic_shading.mesh_strength[exp_level - 1],
                    get_pipe_params()[index].dynamic_shading.exp_thresh_ltoh[exp_level],
                    get_pipe_params()[index].dynamic_shading.mesh_strength[exp_level]);
        }

        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_mlsc_set_attr(pipe_hdl, &shading_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_clut(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_clut != OT_TRUE) || (iso == last_iso)) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_u32 iso_level;
    ot_u32 iso_count = get_pipe_params()[index].dynamic_clut.iso_count;
    ot_u32 *piso_level_thresh = get_pipe_params()[index].dynamic_clut.iso_level;
    ot_u32 gain_r, gain_g, gain_b;
    ot_eis_vi_clut_attr isp_clut_attr;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_clut_get_attr(pipe_hdl, &isp_clut_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    iso_level = scene_get_level_ltoh_u32((ot_u32)iso, iso_count, piso_level_thresh);
    ot_scenecomm_expr_true_return(iso_level >= OT_EIS_SCENE_ISO_STRENGTH_NUM, OT_FAILURE);
    if ((iso_level == 0) || (iso_level == (iso_count - 1))) {
        gain_r = get_pipe_params()[index].dynamic_clut.gain_r[iso_level];
        gain_g = get_pipe_params()[index].dynamic_clut.gain_g[iso_level];
        gain_b = get_pipe_params()[index].dynamic_clut.gain_b[iso_level];
    } else {
        gain_r = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_clut.gain_r[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_clut.gain_r[iso_level]);

        gain_g = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_clut.gain_g[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_clut.gain_g[iso_level]);

        gain_b = scene_interpulate(iso, piso_level_thresh[iso_level - 1],
            get_pipe_params()[index].dynamic_clut.gain_b[iso_level - 1], piso_level_thresh[iso_level],
            get_pipe_params()[index].dynamic_clut.gain_b[iso_level]);
    }
    isp_clut_attr.gain_r = gain_r;
    isp_clut_attr.gain_g = gain_g;
    isp_clut_attr.gain_b = gain_b;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_clut_set_attr(pipe_hdl, &isp_clut_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_clut_auto(ot_vi_pipe_handle pipe_hdl, ot_eis_scene_clut_param *clut_param, ot_u8 index)
{
    //dynamic clut
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_clut != OT_TRUE)) {
        return OT_SUCCESS;
    }

    ot_s32 ret = ot_eis_scene_clut_set_param(pipe_hdl, clut_param, get_pipe_params()[index].dynamic_clut.clut_table, SCENE_DYNAMIC_CLUT_CNT);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_linear_ca(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_linear_ca != OT_TRUE) || (iso == last_iso)) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_s32 i;
    ot_eis_vi_ca_attr ca_attr;
    ot_u32 ca_y_ratio_iso_interp_lut[OT_EIS_VI_CA_YRATIO_LUT_LENGTH];

    ot_u32  iso_count = get_pipe_params()[index].dynamic_ca.iso_count;
    ot_u32 *iso_level_thresh = get_pipe_params()[index].dynamic_ca.iso_level;
    ot_u32 iso_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)iso, iso_count, iso_level_thresh);

    ot_scenecomm_expr_true_return(iso_level >= ISP_AUTO_ISO_DPC_NUM, OT_FAILURE);

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_ca_get_attr(pipe_hdl, &ca_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    if ((iso_level == (iso_count - 1)) || (iso_level == 0)) {
        for (i = 0; i < OT_EIS_VI_CA_YRATIO_LUT_LENGTH; i++) {
            ca_y_ratio_iso_interp_lut[i] = get_pipe_params()[index].dynamic_ca.ca_y_ratio_iso_lut[iso_level][i];
        }
    } else {
        ot_u32 mid_iso = (ot_u32)iso;
        ot_u32 left_iso = iso_level_thresh[iso_level - 1];
        ot_u32 right_iso = iso_level_thresh[iso_level];
        for (i = 0; i < OT_EIS_VI_CA_YRATIO_LUT_LENGTH; i++) {
            ca_y_ratio_iso_interp_lut[i] = scene_interpulate(mid_iso, left_iso,
                get_pipe_params()[index].dynamic_ca.ca_y_ratio_iso_lut[iso_level - 1][i], right_iso,
                get_pipe_params()[index].dynamic_ca.ca_y_ratio_iso_lut[iso_level][i]);
        }
    }

    for (i = 0; i < OT_EIS_VI_CA_YRATIO_LUT_LENGTH; i++) {
        ca_attr.ca.y_ratio_lut[i] = ca_y_ratio_iso_interp_lut[i];
    }

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_ca_set_attr(pipe_hdl, &ca_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

static ot_s32  ot_scene_set_dynamic_ca_part1(ot_eis_vi_ca_attr *ca_attr, ot_u8 index, ot_u32 wdr_ratio, ot_u32 iso)
{
    ot_s32 i;
    ot_u32 ca_y_ratio_iso_interp_lut[OT_EIS_VI_CA_YRATIO_LUT_LENGTH];
    ot_u32 ca_y_ratio_ratio_interp_lut[OT_EIS_VI_CA_YRATIO_LUT_LENGTH];
    ot_u32 blend_weight;
    ot_u32  iso_count = get_pipe_params()[index].dynamic_ca.iso_count;
    ot_u32 *iso_level_thresh = get_pipe_params()[index].dynamic_ca.iso_level;
    ot_u32 iso_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)iso, iso_count, iso_level_thresh);

    ot_u32  ratio_count = get_pipe_params()[index].dynamic_ca.ratio_count;
    ot_u32 *ratio_level_thresh = get_pipe_params()[index].dynamic_ca.ratio_level;
    ot_u32 ratio_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)wdr_ratio, ratio_count, ratio_level_thresh);
    ot_scenecomm_expr_true_return(iso_level >= ISP_AUTO_ISO_DPC_NUM, OT_FAILURE);

    if ((iso_level == (iso_count - 1)) || (iso_level == 0)) {
        for (i = 0; i < OT_EIS_VI_CA_YRATIO_LUT_LENGTH; i++) {
            ca_y_ratio_iso_interp_lut[i] = get_pipe_params()[index].dynamic_ca.ca_y_ratio_iso_lut[iso_level][i];
        }
    } else {
        ot_u32 mid_iso = (ot_u32)iso;
        ot_u32 left_iso = iso_level_thresh[iso_level - 1];
        ot_u32 right_iso = iso_level_thresh[iso_level];
        for (i = 0; i < OT_EIS_VI_CA_YRATIO_LUT_LENGTH; i++) {
            ca_y_ratio_iso_interp_lut[i] = scene_interpulate(mid_iso, left_iso,
                get_pipe_params()[index].dynamic_ca.ca_y_ratio_iso_lut[iso_level - 1][i], right_iso,
                get_pipe_params()[index].dynamic_ca.ca_y_ratio_iso_lut[iso_level][i]);
        }
    }

    if ((ratio_level == (ratio_count - 1)) || (ratio_level == 0)) {
        blend_weight = get_pipe_params()[index].dynamic_ca.blend_weight[ratio_level];
        for (i = 0; i < OT_EIS_VI_CA_YRATIO_LUT_LENGTH; i++) {
            ca_y_ratio_ratio_interp_lut[i] = get_pipe_params()[index].dynamic_ca.ca_y_ratio_ratio_lut[ratio_level][i];
        }
    } else {
        ot_u32 mid_ratio = (ot_u32)wdr_ratio;
        ot_u32 left_ratio = ratio_level_thresh[ratio_level - 1];
        ot_u32 right_ratio = ratio_level_thresh[ratio_level];
        blend_weight = scene_interpulate(mid_ratio, left_ratio,
            get_pipe_params()[index].dynamic_ca.blend_weight[ratio_level - 1], right_ratio,
            get_pipe_params()[index].dynamic_ca.blend_weight[ratio_level]);
        for (i = 0; i < OT_EIS_VI_CA_YRATIO_LUT_LENGTH; i++) {
            ca_y_ratio_ratio_interp_lut[i] = scene_interpulate(mid_ratio, left_ratio,
                get_pipe_params()[index].dynamic_ca.ca_y_ratio_ratio_lut[ratio_level - 1][i], right_ratio,
                get_pipe_params()[index].dynamic_ca.ca_y_ratio_ratio_lut[ratio_level][i]);
        }
    }

    for (i = 0; i < OT_EIS_VI_CA_YRATIO_LUT_LENGTH; i++) {
        ca_attr->ca.y_ratio_lut[i] = (blend_weight * ca_y_ratio_iso_interp_lut[i] +
            (ISP_SCENE_CA_MAX_BLEND_WEIGHT - blend_weight) *ca_y_ratio_ratio_interp_lut[i]) >> ISP_SCENE_CA_SHIFT;
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_ca(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u32 wdr_ratio, ot_u32 iso)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_ca != OT_TRUE)) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_eis_vi_ca_attr ca_attr;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_ca_get_attr(pipe_hdl, &ca_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_dynamic_ca_part1(&ca_attr, index, wdr_ratio, iso);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_ca_set_attr(pipe_hdl, &ca_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_video_gamma(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if (get_pipe_params()[index].module_state.dynamic_gamma != OT_TRUE) {
        return OT_SUCCESS;
    }

    ot_u32 i, j;
    ot_u32 exp_level;
    static ot_u32 last_exp_level[OT_EIS_VI_MAX_PIPE_NUM] = {0};
    ot_s32 ret;
    ot_eis_vi_gamma_attr isp_gamma_attr;
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    if (exposure != last_exposure) {
        exp_level = scene_get_level_ltoh(exposure, get_pipe_params()[index].dynamic_gamma.total_num,
            get_pipe_params()[index].dynamic_gamma.exp_thresh_htol);

        ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
        ot_scenecomm_expr_true_return(exp_level >= OT_SCENE_GAMMA_EXPOSURE_MAX_COUNT, OT_FAILURE);
        for (i = 0; i < get_pipe_params()[index].dynamic_gamma.interval; i++) {
            check_scene_return_if_pause(pipe_hdl);
            ret = ot_eis_vi_gamma_get_attr(pipe_hdl, &isp_gamma_attr);
            ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

            for (j = 0; j < OT_EIS_VI_GAMMA_NODE_NUM; j++) {
                isp_gamma_attr.table[j] =
                    scene_time_filter(get_pipe_params()[index].dynamic_gamma.table[last_exp_level[master_pipe_id]][j],
                        get_pipe_params()[index].dynamic_gamma.table[exp_level][j],
                        get_pipe_params()[index].dynamic_gamma.interval, i);
            }

            isp_gamma_attr.curve_type = OT_EIS_VI_GAMMA_CURVE_USER_DEFINE;
            check_scene_return_if_pause(pipe_hdl);
            ret = ot_eis_vi_gamma_set_attr(pipe_hdl, &isp_gamma_attr);
            ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
            usleep(30000); /* sleep 30000 us */
        }
        last_exp_level[master_pipe_id] = exp_level;
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_photo_gamma(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index)
{
    return OT_SUCCESS;
}


ot_s32 ot_scene_set_dynamic_false_color(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_false_color != OT_TRUE) || (last_exposure == exposure)) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_u32 exp_level;
    ot_eis_vi_anti_false_color_attr anti_false_color_attr;

    if (exposure != last_exposure) {
        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_anti_false_color_get_attr(pipe_hdl, &anti_false_color_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

        exp_level = scene_get_level_ltoh_u32((ot_u32)exposure, get_pipe_params()[index].dynamic_false_color.total_num,
            get_pipe_params()[index].dynamic_false_color.false_color_exp_thresh);
        ot_scenecomm_expr_true_return(exp_level >= OT_SCENE_FALSECOLOR_EXPOSURE_MAX_COUNT, OT_FAILURE);
        anti_false_color_attr.manual_attr.strength =
            get_pipe_params()[index].dynamic_false_color.manual_strength[exp_level];

        check_scene_return_if_pause(pipe_hdl);
        ret = ot_eis_vi_anti_false_color_set_attr(pipe_hdl, &anti_false_color_attr);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    }
    return OT_SUCCESS;
}


static ot_bool g_fpn_enable[OT_EIS_VI_MAX_PIPE_NUM] = {OT_FALSE};

ot_s32 ot_scene_release_fpn(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret;
    if (get_pipe_params()[index].module_state.dynamic_fpn != OT_TRUE) {
        return OT_SUCCESS;
    }
    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    if (g_fpn_enable[master_pipe_id] == OT_TRUE) {
        g_fpn_enable[master_pipe_id] = OT_FALSE;
        ret = sample_comm_vi_disable_fpn_correction(pipe_hdl, &g_correction_cfg);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
        scene_logd("release fpn buffer ok!\n");
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_fpn_decide(ot_vi_pipe_handle pipe_hdl, ot_u32 *iso_tmp, ot_u32 *last_iso_tmp,
                                   ot_bool *fpn_enable, ot_u8 index)
{
    ot_s32 ret;
    ot_u32 i, delta, sns_iso;
    ot_eis_vi_exp_info exp_info;
    ot_u32 iso_count = get_pipe_params()[index].dynamic_fpn.iso_count;
    ot_u32 *iso_level_thresh = get_pipe_params()[index].dynamic_fpn.iso_thresh;
    ot_u32 fpn_iso_thresh = get_pipe_params()[index].dynamic_fpn.fpn_iso_thresh;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_ae_query_info(pipe_hdl, &exp_info);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    sns_iso = ((ot_u64)exp_info.a_gain * exp_info.d_gain * FPN_GAIN_ISO) >> FPN_GAIN_SHIFT;

    if (sns_iso >= iso_level_thresh[iso_count - 1]) {
        *iso_tmp = iso_level_thresh[iso_count - 1];
        *fpn_enable = OT_TRUE;
    } else if (sns_iso < fpn_iso_thresh) {
        *iso_tmp = sns_iso;
        if (*fpn_enable == OT_TRUE) {
            *fpn_enable = OT_FALSE;
            ret = sample_comm_vi_disable_fpn_correction(pipe_hdl, &g_correction_cfg);
            ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
            *last_iso_tmp = *iso_tmp;
            scene_logd("disable fpn correction\n");
        }
    } else {
        if ((iso_count - 1) == 0) {
            *iso_tmp = iso_level_thresh[0];
        }
        for (i = 0; i < iso_count - 1; i++) {
            delta = (iso_level_thresh[i + 1] - iso_level_thresh[i]) >> 1;
            if (sns_iso <= (iso_level_thresh[i] + delta)) {
                *iso_tmp = iso_level_thresh[i];
                break;
            } else if ((sns_iso > (iso_level_thresh[i] + delta)) && (sns_iso < iso_level_thresh[i + 1])) {
                *iso_tmp = iso_level_thresh[i + 1];
                break;
            }
        }
        *fpn_enable = OT_TRUE;
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_fpn(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index)
{
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_fpn != OT_TRUE) || (last_iso == iso)) {
        return OT_SUCCESS;
    }

    ot_s32 ret;
    ot_u32 iso_level, sns_iso;
    ot_u32 iso_tmp = 0;
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    static ot_u32 last_iso_tmp[OT_EIS_VI_MAX_PIPE_NUM] = {0};
    ot_eis_vi_exp_info exp_info;
    ot_u32 iso_count = get_pipe_params()[index].dynamic_fpn.iso_count;
    ot_u32 *iso_level_thresh = get_pipe_params()[index].dynamic_fpn.iso_thresh;
    sample_scene_fpn_offset_cfg  scene_fpn_offset_cfg;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_ae_query_info(pipe_hdl, &exp_info);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    sns_iso = ((ot_u64)exp_info.a_gain * exp_info.d_gain * FPN_GAIN_ISO) >> FPN_GAIN_SHIFT;
    iso_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)sns_iso, iso_count, iso_level_thresh);
    ot_scenecomm_expr_true_return(iso_level >= OT_EIS_SCENE_ISO_STRENGTH_NUM, OT_FAILURE);

    scene_fpn_offset_cfg.offset = get_pipe_params()[index].dynamic_fpn.fpn_offset[iso_level];
    g_correction_cfg.aibnr_mode = get_pipe_params()[index].dynamic_fpn.aibnr_mode;

    ret = ot_scene_dynamic_fpn_decide(pipe_hdl, &iso_tmp, &last_iso_tmp[master_pipe_id], &g_fpn_enable[master_pipe_id], index);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    if (last_iso_tmp[master_pipe_id] != iso_tmp && (g_fpn_enable[master_pipe_id] == OT_TRUE)) {
        ret = sample_comm_vi_enable_fpn_correction_for_scene(pipe_hdl, &g_correction_cfg, iso_tmp,
                                                             &scene_fpn_offset_cfg, g_dir_name);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
        last_iso_tmp[master_pipe_id] = iso_tmp;
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_abnr(ot_vi_pipe_handle pipe_hdl, ot_u8 index)
{
    ot_s32 ret;
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_abnr != OT_TRUE)) {
        return OT_SUCCESS;
    }

    ot_eis_vi_exp_info exp_info = { 0 };
    ret = ot_eis_vi_ae_query_info(pipe_hdl, &exp_info);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_eis_scene_abnr_set_param(pipe_hdl, exp_info.iso);
    ot_scenecomm_expr_true_return(ret != OT_SUCCESS && ret != OT_EIS_ERRNO_NO_INIT, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_iqt(ot_vi_pipe_handle pipe_hdl, ot_eis_handle task_hdl, ot_eis_video_frame *video_frame, ot_u8 index)
{
    ot_s32 ret;
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    if ((get_pipe_params()[index].module_state.dynamic_iqt != OT_TRUE)) {
        return OT_SUCCESS;
    }

    ret = ot_eis_scene_iqt_process(task_hdl, video_frame);
    ot_scenecomm_expr_true_return(ret != OT_SUCCESS, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_mce(ot_vi_pipe_handle pipe_hdl, ot_eis_video_frame *video_frame, ot_u8 index)
{
    ot_s32 ret;
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    ot_scenecomm_check_pointer_return(video_frame, OT_SCENE_ENONPTR);
    if ((get_pipe_params()[index].module_state.dynamic_mce != OT_TRUE)) {
        return OT_SUCCESS;
    }

    ot_eis_vi_exp_info exp_info = { 0 };
    ret = ot_eis_vi_ae_query_info(pipe_hdl, &exp_info);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    if (exp_info.iso < get_pipe_params()[index].dynamic_mce.mce_iso_thresh) {
        ret = ot_eis_scene_mce_set_param(pipe_hdl, video_frame);
        ot_scenecomm_expr_true_return(ret != OT_SUCCESS && ret != OT_EIS_ERRNO_NO_INIT, OT_SCENE_EINTER);
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_dynamic_3dnr(ot_eis_handle vi_pipe_hdl, ot_eis_handle vproc_hdl, ot_u32 iso, ot_u8 index, ot_eis_3dnr_pos pos_3dnr)
{
    if (get_pipe_params()[index].module_state.dynamic_3dnr != OT_TRUE) {
        return OT_SUCCESS;
    }
    check_scene_return_if_pause(vi_pipe_hdl);
    ot_u32 iso_level = 0;
    ot_scene_3dnr nrx_attr;
    const ot_scene_static_3dnr *nrx_param = &(get_pipe_params()[index].static_threednr);
    ot_u32 count = nrx_param->threed_nr_count;
    ot_u32 *thresh = (ot_u32 *)nrx_param->threed_nr_iso;
    iso_level = scene_get_level_ltoh_u32(iso, count, thresh);
    if (iso_level == 0) {
        nrx_attr = nrx_param->threednr_value[0];
    } else {
        ot_scenecomm_expr_true_return(iso_level >= OT_SCENE_3DNR_MAX_COUNT, OT_FAILURE);
        ot_u32 mid = iso;
        ot_u32 left = thresh[iso_level - 1];
        ot_u32 right = thresh[iso_level];

        nrx_attr = nrx_param->threednr_value[iso_level];

        _3dnr_nrx_pack pack = { &nrx_attr, nrx_param, iso_level, mid, left, right };

        scene_set_3dnr_nrx_adv_iey(&pack);

        scene_set_3dnr_nrx_adv_sfy(&pack);

        scene_set_3dnr_nrx_nry(&pack);

        scene_set_3dnr_nrx_iey(&pack);

        scene_set_3dnr_nrx_sfy(&pack);

        scene_set_3dnr_nrx_tfy(&pack, pos_3dnr);

        scene_set_3dnr_nrx_mdy(&pack);

        scene_set_3dnr_nrx_nrc0(&pack);

        scene_set_3dnr_nrx_nrc1(&pack);
    }
    check_scene_return_if_pause(vi_pipe_hdl);
    ot_s32 ret = scene_set_3dnr(vi_pipe_hdl, vproc_hdl, &nrx_attr, index, pos_3dnr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_pipe_param(const ot_scene_pipe_param *scene_pipe_param, ot_u32 num)
{
    ot_scenecomm_check_pointer_return(scene_pipe_param, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(num > OT_SCENE_PIPETYPE_NUM, OT_SCENE_EOUTOFRANGE);

    errno_t ret = memcpy_s(get_pipe_params(), sizeof(ot_scene_pipe_param) * OT_SCENE_PIPETYPE_NUM, scene_pipe_param,
                           sizeof(ot_scene_pipe_param) * num);
    if (ret != EOK) {
        scene_loge("copy scene pipe params fail. num = %u\n", num);
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_set_pause(ot_vi_pipe_handle pipe_hdl, ot_bool pause)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    master_pipe_id = pipe_id[0];
    get_isp_pause()[master_pipe_id] = pause;
    return OT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
