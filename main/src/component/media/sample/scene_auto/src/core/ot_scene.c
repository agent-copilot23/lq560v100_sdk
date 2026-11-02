/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#include "ot_scene.h"
#include "securec.h"
#include "ot_scene_inner.h"
#include "ot_scenecomm.h"
#include "ot_scene_setparam.h"
#include "ot_eis_vi_ae_api.h"
#include "ot_eis_vi_awb_api.h"
#include "ot_eis_vi_alg_api.h"
#include "ot_eis_media_sys.h"
#include "ot_scenecomm_log.h"
#include "scene_setparam_inner.h"
#include "ot_scene_inner.h"
#include "ot_scene.h"
#include "ot_eis_vi_api.h"
#include "ot_eis_scene_auto_api.h"
#include "sample_comm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* notes scene state */
static ot_scene_state g_scene_state = {0};

/* scene lock */
static pthread_mutex_t g_scene_lock = PTHREAD_MUTEX_INITIALIZER;

/* clut param */
static ot_scene_clut_param g_clut_param[OT_SCENE_PIPE_MAX_NUM] = {0};

/* -------------------------internal function interface------------------------- */
static ot_void scene_get_last_exposure(scene_thread_type thread_type, ot_s32 i, ot_u64 *last_exposure)
{
    switch (thread_type) {
        case SCENE_THREAD_TYPE_NORMAL:
            *last_exposure = g_scene_state.exp_info[i].exp_last_normal.exposure;
            break;
        case SCENE_THREAD_TYPE_DEHAZE:
            *last_exposure = g_scene_state.exp_info[i].exp_last_dehaze.exposure;
            break;
        default:
            scene_loge("Error thread_type\n");
            break;
    }
}

static ot_void scene_get_last_iso(scene_thread_type thread_type, ot_s32 i, ot_u32 *last_iso)
{
    switch (thread_type) {
        case SCENE_THREAD_TYPE_NORMAL:
            *last_iso = g_scene_state.exp_info[i].exp_last_normal.iso;
            break;
        case SCENE_THREAD_TYPE_DEHAZE:
            *last_iso = g_scene_state.exp_info[i].exp_last_dehaze.iso;
            break;
        default:
            scene_loge("Error thread_type\n");
            break;
    }
}

static ot_void scene_update_last_exposure_info(scene_thread_type thread_type, ot_s32 pipe_id)
{
    switch (thread_type) {
        case SCENE_THREAD_TYPE_NORMAL:
            g_scene_state.exp_info[pipe_id].exp_last_normal.exposure = g_scene_state.exp_info[pipe_id].exp_cur.exposure;
            g_scene_state.exp_info[pipe_id].exp_last_normal.iso = g_scene_state.exp_info[pipe_id].exp_cur.iso;
            break;
        case SCENE_THREAD_TYPE_DEHAZE:
            g_scene_state.exp_info[pipe_id].exp_last_dehaze.exposure = g_scene_state.exp_info[pipe_id].exp_cur.exposure;
            g_scene_state.exp_info[pipe_id].exp_last_dehaze.iso = g_scene_state.exp_info[pipe_id].exp_cur.iso;
            break;
        default:
            scene_loge("Error thread_type\n");
            break;
    }
}

static ot_s32 scene_calculate_exp(ot_vi_pipe_handle pipe_hdl, ot_u32 *out_iso, ot_u64 *out_exposure)
{
    ot_s32 ret = OT_FAILURE;
    ot_u64 exposure;
    ot_u32 iso;

    ot_eis_vi_exp_info isp_exp_info;
    ot_eis_vi_public_attr pub_attr;

    check_scene_return_if_pause(pipe_hdl);

    ret = ot_eis_vi_ae_query_info(pipe_hdl, &isp_exp_info);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_eis_vi_public_get_attr(pipe_hdl, &pub_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    iso = isp_exp_info.iso;

    if (pub_attr.wdr_mode == OT_EIS_WDR_MODE_4To1_LINE) {
        exposure = ((ot_u64)iso * isp_exp_info.long_exp_time) / 100; /* 100 as AE ISO ratio */
    } else if (pub_attr.wdr_mode == OT_EIS_WDR_MODE_3To1_LINE) {
        exposure = ((ot_u64)iso * isp_exp_info.median_exp_time) / 100; /* 100 as AE ISO ratio */
    } else if (pub_attr.wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        exposure = ((ot_u64)iso * isp_exp_info.short_exp_time) / 100; /* 100 as AE ISO ratio */
    } else if (pub_attr.wdr_mode == OT_EIS_WDR_MODE_2To1_FRAME) {
        exposure = ((ot_u64)iso * isp_exp_info.short_exp_time) / 100; /* 100 as AE ISO ratio */
    } else {
        exposure = ((ot_u64)iso * isp_exp_info.exp_time) / 100; /* 100 as AE ISO ratio */
    }
    *out_iso = iso;
    *out_exposure = exposure;

    return OT_SUCCESS;
}


static ot_s32 scene_set_dynamic_param_by_param(scene_set_dynamic_by_param_cb func_param,
        scene_thread_type thread_type, scene_dynamic_change_type dynamic_change_type, ot_vi_pipe_handle pipe_hdl)
{
    ot_scenecomm_check_pointer_return(func_param, OT_FAILURE);

    ot_s32 ret = OT_FAILURE;

    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ot_u32 index = g_scene_state.pipe_param_idx[master_pipe_id];

    if (dynamic_change_type == SCENE_DYNAMIC_CHANGE_TYPE_EXPOSURE) {
        ot_u64 exposure = g_scene_state.exp_info[master_pipe_id].exp_cur.exposure;
        ot_u64 last_exposure = 0;
        scene_get_last_exposure(thread_type, master_pipe_id, &last_exposure);

        ret = func_param(pipe_hdl, exposure, last_exposure, index);
    } else {
        ot_u32 iso = g_scene_state.exp_info[master_pipe_id].exp_cur.iso;
        ot_u32 last_iso = 0;
        scene_get_last_iso(thread_type, master_pipe_id, &last_iso);

        ret = func_param(pipe_hdl, iso, last_iso, index);
    }
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

static ot_s32 scene_calculate_wdr_param(ot_vi_pipe_handle pipe_hdl, ot_u32 *wdr_ratio)
{
    ot_s32 ret = OT_FAILURE;
    ot_eis_vi_alg_state_info inner_state_info;

    check_scene_return_if_pause(pipe_hdl);

    ret = ot_eis_vi_query_alg_state_info(pipe_hdl, &inner_state_info);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    *wdr_ratio = inner_state_info.wdr_exp_ratio_actual[0];

    return OT_SUCCESS;
}

static ot_s32 scene_set_main_pipe_special_param(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ot_u32 index = g_scene_state.pipe_param_idx[master_pipe_id];
    ret = ot_scene_set_static_ae(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_back_light(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_gamma(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_awb(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_mce(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_awbex(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_saturation(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_wdr_exposure(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_sharpen(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_demosaic(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_bayershp(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_ca(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_crosstalk(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_clut(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_fswdr(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_drc(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_shading(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_blc(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

static ot_s32 scene_set_pipe_static_param(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);   

    ot_u32 index = g_scene_state.pipe_param_idx[master_pipe_id];

    ret = ot_scene_set_static_ccm(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_nr(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_csc(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_ldci(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_ldcihlc(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_cac(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_dpc(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_static_dehaze(pipe_hdl, index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}


static ot_s32 scene_set_pipe_dynamic_param_part1(ot_vi_pipe_handle pipe_hdl)
{
    scene_set_dynamic_by_param_cb func_false_color = ot_scene_set_dynamic_false_color;
    ot_s32 ret = scene_set_dynamic_param_by_param(func_false_color, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_EXPOSURE, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    scene_set_dynamic_by_param_cb func_shading = ot_scene_set_dynamic_shading;
    ret = scene_set_dynamic_param_by_param(func_shading, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_EXPOSURE, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    scene_set_dynamic_by_param_cb func_dpc = ot_scene_set_dynamic_dpc;
    ret = scene_set_dynamic_param_by_param(func_dpc, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_ISO, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    scene_set_dynamic_by_param_cb func_awb = ot_scene_set_dynamic_awb;
    ret = scene_set_dynamic_param_by_param(func_awb, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_ISO, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    scene_set_dynamic_by_param_cb func_clut = ot_scene_set_dynamic_clut;
    ret = scene_set_dynamic_param_by_param(func_clut, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_ISO, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    scene_set_dynamic_by_param_cb func_linear_ca = ot_scene_set_dynamic_linear_ca;
    ret = scene_set_dynamic_param_by_param(func_linear_ca, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_ISO, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    scene_set_dynamic_by_param_cb func_ldci = ot_scene_set_dynamic_ldci;
    ret = scene_set_dynamic_param_by_param(func_ldci, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_EXPOSURE, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    scene_set_dynamic_by_param_cb func_ldcihlc = ot_scene_set_dynamic_ldcihlc;
    ret = scene_set_dynamic_param_by_param(func_ldcihlc, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_EXPOSURE, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    scene_set_dynamic_by_param_cb func_back_light = ot_scene_set_dynamic_back_light;
    ret = scene_set_dynamic_param_by_param(func_back_light, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_ISO, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

static ot_s32 scene_dynamic_not_linear_set_param(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ot_eis_vi_public_attr pub_attr = {0};
    ret = ot_eis_vi_public_get_attr(pipe_hdl, &pub_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    if (pub_attr.wdr_mode > OT_EIS_WDR_MODE_RESERVED && pub_attr.wdr_mode < OT_EIS_WDR_MODE_BUTT) {
        ret = scene_calculate_wdr_param(pipe_hdl, &(g_scene_state.exp_info[master_pipe_id].wdr_ratio));
        ot_scenecomm_check(ret, OT_SCENE_EINTER);

        ret = ot_scene_set_dynamic_fswdr(pipe_hdl, g_scene_state.exp_info[master_pipe_id].exp_cur.iso,
                g_scene_state.exp_info[master_pipe_id].exp_last_normal.iso, g_scene_state.pipe_param_idx[master_pipe_id],
                g_scene_state.exp_info[master_pipe_id].wdr_ratio);
        ot_scenecomm_check(ret, OT_SCENE_EINTER);

        ret = ot_scene_set_dynamic_drc(pipe_hdl, g_scene_state.pipe_param_idx[master_pipe_id],
                g_scene_state.exp_info[master_pipe_id].wdr_ratio, g_scene_state.exp_info[master_pipe_id].exp_cur.iso);
        ot_scenecomm_check(ret, OT_SCENE_EINTER);

        ret = ot_scene_set_dynamic_nr(pipe_hdl, g_scene_state.pipe_param_idx[master_pipe_id],
                g_scene_state.exp_info[master_pipe_id].wdr_ratio, g_scene_state.exp_info[master_pipe_id].exp_cur.iso);
        ot_scenecomm_check(ret, OT_SCENE_EINTER);

        ret = ot_scene_set_dynamic_ca(pipe_hdl, g_scene_state.pipe_param_idx[master_pipe_id],
                g_scene_state.exp_info[master_pipe_id].wdr_ratio, g_scene_state.exp_info[master_pipe_id].exp_cur.iso);
        ot_scenecomm_check(ret, OT_SCENE_EINTER);
    }

    return OT_SUCCESS;
}

static ot_s32 scene_set_auto_luminance_gamma(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ot_u8 index = g_scene_state.pipe_param_idx[master_pipe_id];
    ot_u64 exposure = g_scene_state.exp_info[master_pipe_id].exp_cur.exposure;

    if (g_scene_state.pipe_type[master_pipe_id] == OT_SCENE_PIPE_TYPE_SNAP) {
        ret = ot_scene_set_dynamic_photo_gamma(pipe_hdl, exposure,
            g_scene_state.exp_info[master_pipe_id].exp_last_normal.exposure, index);
        ot_scenecomm_check(ret, OT_SCENE_EINTER);
    } else if (g_scene_state.pipe_type[master_pipe_id] == OT_SCENE_PIPE_TYPE_VIDEO) {
        ret = ot_scene_set_dynamic_video_gamma(pipe_hdl, exposure,
            g_scene_state.exp_info[master_pipe_id].exp_last_normal.exposure, index);
        ot_scenecomm_check(ret, OT_SCENE_EINTER);
    }   

    return OT_SUCCESS;
}

static ot_s32 scene_dynamic_luminance_set_param(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("luminance set param failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    ret = scene_set_dynamic_param_by_param(ot_scene_set_dynamic_fps, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_EXPOSURE, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);
    
    ret = scene_set_auto_luminance_gamma(pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    if (g_scene_state.pipe_type[master_pipe_id] == OT_SCENE_PIPE_TYPE_SNAP) {
        ret = scene_set_dynamic_param_by_param(ot_scene_set_dynamic_photo_gamma, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_EXPOSURE, pipe_hdl);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    } else if (g_scene_state.pipe_type[master_pipe_id] == OT_SCENE_PIPE_TYPE_VIDEO) {
        ret = scene_set_dynamic_param_by_param(ot_scene_set_dynamic_video_gamma, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_EXPOSURE, pipe_hdl);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    }

    return OT_SUCCESS;
}

static ot_s32 scene_set_pipe_dynamic_param(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_grp_hdl)
{
    ot_s32 ret = OT_FAILURE;
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    // ot_u32 iso;
    ot_eis_3dnr_pos pos = OT_EIS_3DNR_POS_VI;
    ret = ot_eis_media_get_3dnr_pos(&pos);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = scene_set_pipe_dynamic_param_part1(pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    scene_set_dynamic_by_param_cb func_fpn = ot_scene_set_dynamic_fpn;
    ret = scene_set_dynamic_param_by_param(func_fpn, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_ISO, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ot_u8 pipe_index = g_scene_state.pipe_param_idx[master_pipe_id];
    if (g_scene_state.pipe_type[master_pipe_id] != OT_SCENE_PIPE_TYPE_SNAP) {
        // iso = g_scene_state.exp_info[i].exp_cur.iso;
        // ot_u64 exposure = g_scene_state.exp_info[master_pipe_id].exp_cur.exposure / 100; //30ms -> *3.0     
        ot_u64 exposure = g_scene_state.exp_info[master_pipe_id].exp_cur.exposure;           
        ret = ot_scene_set_dynamic_3dnr(pipe_hdl, vproc_grp_hdl, exposure, pipe_index, pos);
        ot_scenecomm_check(ret, OT_SCENE_EINTER);       
    }

    // //iso -> exposure
    ot_u32 iso = g_scene_state.exp_info[master_pipe_id].exp_cur.iso;    //main pipe iso?
    ret = ot_scene_set_dynamic_drc(pipe_hdl, pipe_index, 64, iso);    //wdr_ratio 64 tbd
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = ot_scene_set_dynamic_vproc(vproc_grp_hdl, iso, 64, pipe_index);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);        
    
    scene_set_dynamic_by_param_cb func_ae = ot_scene_set_dynamic_ae;
    ret = scene_set_dynamic_param_by_param(func_ae, SCENE_THREAD_TYPE_NORMAL, SCENE_DYNAMIC_CHANGE_TYPE_EXPOSURE, pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = scene_dynamic_luminance_set_param(pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = scene_dynamic_not_linear_set_param(pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_static_vi_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_grp_hdl)
{
    ot_s32 ret = OT_FAILURE;
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ot_mutex_lock(g_scene_lock);

    /* set mainIsp param */
    ret = scene_set_main_pipe_special_param(pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ot_u8 index = g_scene_state.pipe_param_idx[master_pipe_id];
    if (g_scene_state.pipe_type[master_pipe_id] == OT_SCENE_PIPE_TYPE_VIDEO) {
        ret = ot_scene_set_static_3dnr(pipe_hdl, vproc_grp_hdl, index);
        ot_scenecomm_check(ret, OT_SCENE_EINTER);
    }

    ret = scene_set_pipe_static_param(pipe_hdl);
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = scene_calculate_exp(pipe_hdl, &(g_scene_state.exp_info[master_pipe_id].exp_cur.iso),
                    &(g_scene_state.exp_info[master_pipe_id].exp_cur.exposure));
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ot_mutex_unlock(g_scene_lock);

    return OT_SUCCESS;
}

ot_s32 ot_scene_ctrl_clut_get_param(ot_vi_pipe_handle pipe_hdl, ot_scene_ctrl_param_clut *ctrl_param)
{
    // TODO
    ot_scenecomm_check_pointer_return(ctrl_param, OT_SCENE_ENONPTR);
    return OT_SUCCESS;
}

ot_s32 ot_scene_ctrl_clut_set_param(ot_vi_pipe_handle pipe_hdl, const ot_scene_ctrl_param_clut *ctrl_param)
{
    ot_scenecomm_check_pointer_return(ctrl_param, OT_SCENE_ENONPTR);
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ot_s32 index = g_scene_state.pipe_param_idx[master_pipe_id];
    ret = ot_scene_set_static_clut_param(pipe_hdl, index, ctrl_param->ct_value);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);  
    
    return OT_SUCCESS;
}

ot_s32 ot_scene_ctrl_dehaze_get_param(ot_vi_pipe_handle pipe_hdl, ot_scene_ctrl_param_dehaze *ctrl_param)
{
    // TODO
    ot_scenecomm_check_pointer_return(ctrl_param, OT_SCENE_ENONPTR);
    return OT_SUCCESS;
}

ot_s32 ot_scene_ctrl_dehaze_set_param(ot_vi_pipe_handle pipe_hdl, const ot_scene_ctrl_param_dehaze *ctrl_param)
{
    ot_scenecomm_check_pointer_return(ctrl_param, OT_SCENE_ENONPTR);

    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ot_s32 index = g_scene_state.pipe_param_idx[master_pipe_id];
    ret = ot_scene_set_static_dehaze_param(pipe_hdl, index, ctrl_param->enable);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER); 
    
    return OT_SUCCESS;
}

ot_s32 ot_scene_ctrl_deflicker_get_param(ot_vi_pipe_handle pipe_hdl, ot_scene_ctrl_param_deflicker *ctrl_param)
{
    // TODO
    ot_scenecomm_check_pointer_return(ctrl_param, OT_SCENE_ENONPTR);
    return OT_SUCCESS;
}

ot_s32 ot_scene_ctrl_deflicker_set_param(ot_vi_pipe_handle pipe_hdl, const ot_scene_ctrl_param_deflicker *ctrl_param)
{
    ot_scenecomm_check_pointer_return(ctrl_param, OT_SCENE_ENONPTR);

    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ot_s32 index = g_scene_state.pipe_param_idx[master_pipe_id];
    ret = ot_scene_set_static_deflicker_param(pipe_hdl, index, ctrl_param->enable);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER); 
    
    return OT_SUCCESS;
}

ot_s32 ot_scene_ctrl_3dnr_get_param(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_grp_hdl, ot_scene_ctrl_param_3dnr *ctrl_param)
{
    ot_scenecomm_check_pointer_return(ctrl_param, OT_SCENE_ENONPTR);
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ot_scenecomm_check_return(g_scene_state.pipe_type[master_pipe_id] == OT_SCENE_PIPE_TYPE_SNAP, OT_SCENE_NO_SUPPORT);

    ot_eis_3dnr_attr nrx_attr = {0};
    ot_u32 param_iso[16] = {0};
    ot_eis_nr_cfg nr_param[16] = {0};

    nrx_attr.param.nr_cfg.nr_auto.iso = param_iso;
    nrx_attr.param.nr_cfg.nr_auto.nr_param = nr_param;

    ot_eis_3dnr_pos pos_3dnr = OT_EIS_3DNR_POS_VI;
    ret = ot_eis_media_get_3dnr_pos(&pos_3dnr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);  

    if (pos_3dnr == OT_EIS_3DNR_POS_VI) {
        ret = ot_eis_vi_pipe_get_3dnr_attr(pipe_hdl,  &nrx_attr);
    } else {
        nrx_attr.param.nr_cfg.nr_mode = OT_EIS_OP_MODE_MANUAL;
        ret = ot_eis_vproc_grp_get_3dnr(vproc_grp_hdl,  &nrx_attr);
    }

    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ctrl_param->enable = nrx_attr.enable;
    ctrl_param->compress_mode = nrx_attr.compress_mode;

    return OT_SUCCESS;
}

ot_s32 ot_scene_ctrl_3dnr_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_grp_hdl, const ot_scene_ctrl_param_3dnr *ctrl_param)
{
    ot_scenecomm_check_pointer_return(ctrl_param, OT_SCENE_ENONPTR);

    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ot_scenecomm_check_return(g_scene_state.pipe_type[master_pipe_id] == OT_SCENE_PIPE_TYPE_SNAP, OT_SCENE_NO_SUPPORT);
    
    ot_s32 index = g_scene_state.pipe_param_idx[master_pipe_id];

    ot_scene_3dnr_param param = {0};
    param.enable = ctrl_param->enable;
    param.set_compress_mode = OT_TRUE;
    param.compress_mode = ctrl_param->compress_mode;
    return ot_scene_set_static_3dnr_param(pipe_hdl, vproc_grp_hdl, &param, index);

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_normal_init(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("normal init failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }

    return OT_SUCCESS;
}


ot_s32 ot_scene_dynamic_normal_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_grp_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("normal set param failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    ret = scene_calculate_exp(pipe_hdl, &(g_scene_state.exp_info[master_pipe_id].exp_cur.iso), 
                    &(g_scene_state.exp_info[master_pipe_id].exp_cur.exposure));
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = scene_set_pipe_dynamic_param(pipe_hdl, vproc_grp_hdl);
    scene_update_last_exposure_info(SCENE_THREAD_TYPE_NORMAL, master_pipe_id);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

        
    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_normal_deinit(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("normal deinit failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    ret = ot_scene_release_fpn(pipe_hdl, g_scene_state.pipe_param_idx[master_pipe_id]);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);        
        
    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_dehaze_init(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("dehaze init failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    if (get_pipe_params()[master_pipe_id].dynamic_dehaze.dehzae_level_enable) {
        ret = ot_eis_scene_haze_level_init(pipe_hdl);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_dehaze_deinit(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("dehaze deinit failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    if (get_pipe_params()[master_pipe_id].dynamic_dehaze.dehzae_level_enable) {
        ret = ot_eis_scene_haze_level_deinit(pipe_hdl);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_dehaze_set_param(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("dehaze set param failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    ret = scene_calculate_exp(pipe_hdl, &(g_scene_state.exp_info[master_pipe_id].exp_cur.iso), 
                &(g_scene_state.exp_info[master_pipe_id].exp_cur.exposure));
    ot_scenecomm_check(ret, OT_SCENE_EINTER);

    ret = scene_set_dynamic_param_by_param(ot_scene_set_dynamic_dehaze, SCENE_THREAD_TYPE_DEHAZE, SCENE_DYNAMIC_CHANGE_TYPE_EXPOSURE, pipe_hdl);    
    scene_update_last_exposure_info(SCENE_THREAD_TYPE_DEHAZE, master_pipe_id);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    
    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_clut_init(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("clut init failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    memset(&g_clut_param[master_pipe_id], 0, sizeof(ot_scene_clut_param));

    ot_eis_vi_clut_attr clut_attr_get = { 0 };
    ot_eis_vi_clut_lut clut_coeff_get = { 0 };
    ot_eis_vi_clut_get_attr(pipe_hdl, &clut_attr_get);
    ot_eis_vi_clut_get_coeff(pipe_hdl, &clut_coeff_get);

    if (clut_attr_get.enable == OT_TRUE) {
        g_clut_param[master_pipe_id].ori_clut_valid = OT_TRUE;
        g_clut_param[master_pipe_id].ori_clut.gain[0] = clut_attr_get.gain_r;
        g_clut_param[master_pipe_id].ori_clut.gain[1] = clut_attr_get.gain_g;
        g_clut_param[master_pipe_id].ori_clut.gain[2] = clut_attr_get.gain_b;
        memcpy_s(g_clut_param[master_pipe_id].ori_clut.lut, sizeof(ot_u32) * OT_EIS_VI_CLUT_LUT_LENGTH,
                clut_coeff_get.lut, sizeof(ot_u32) * OT_EIS_VI_CLUT_LUT_LENGTH);
    } else {
        g_clut_param[master_pipe_id].ori_clut_valid = OT_FALSE;
    }

    for (int i = 0; i < OT_SCENE_PIPE_MAX_NUM; i++) {
        g_clut_param[master_pipe_id].clut_attr.last_trans_coeff[i] = 1;
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_clut_set_param(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("clut set param failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    ot_eis_vi_clut_attr clut_attr_get = { 0 };
    ot_eis_vi_clut_get_attr(pipe_hdl, &clut_attr_get);
    if (clut_attr_get.enable == OT_TRUE) {
        ret = ot_scene_set_dynamic_clut_auto(pipe_hdl, &g_clut_param[master_pipe_id].clut_attr, g_scene_state.pipe_param_idx[master_pipe_id]);
        ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_clut_deinit(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("clut deinit failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    if (g_clut_param[master_pipe_id].ori_clut_valid == OT_TRUE) {
        ot_eis_vi_clut_attr clut_attr_set = { 0 };
        ot_eis_vi_clut_lut clut_coeff_set = { 0 };

        ot_eis_vi_clut_get_attr(pipe_hdl, &clut_attr_set);
        clut_attr_set.gain_r = g_clut_param[master_pipe_id].ori_clut.gain[0];
        clut_attr_set.gain_g = g_clut_param[master_pipe_id].ori_clut.gain[1];
        clut_attr_set.gain_b = g_clut_param[master_pipe_id].ori_clut.gain[2];
        memcpy_s(clut_coeff_set.lut, sizeof(ot_u32) * OT_EIS_VI_CLUT_LUT_LENGTH, g_clut_param[master_pipe_id].ori_clut.lut, sizeof(ot_u32) * OT_EIS_VI_CLUT_LUT_LENGTH);

        ot_eis_vi_clut_set_attr(pipe_hdl, &clut_attr_set);
        ot_eis_vi_clut_set_coeff(pipe_hdl, &clut_coeff_set);
    }

    return OT_SUCCESS;
}



ot_s32 ot_scene_init(const ot_scene_param *scene_param)
{
    ot_scenecomm_check_pointer_return(scene_param, OT_SCENE_ENONPTR);

    ot_s32 ret = OT_FAILURE;

    ot_mutex_lock(g_scene_lock);
    if (g_scene_state.scene_init == OT_TRUE) {
        scene_loge("SCENE module has already been inited.\n");
        ret = OT_SUCCESS;
        goto err_exit;
    }

    ret = ot_scene_set_pipe_param(scene_param->pipe_param, OT_SCENE_PIPETYPE_NUM);
    if (ret != OT_SUCCESS) {
        scene_loge("set pipe param failed.\n");
        ret = OT_SCENE_EINTER;
        goto err_exit;
    }

    (ot_void)memset_s(&g_scene_state, sizeof(g_scene_state), 0, sizeof(g_scene_state));

    for (int i = 0; i < OT_SCENE_PIPE_MAX_NUM; i++) {
        g_scene_state.pipe_type[i] = OT_SCENE_PIPE_TYPE_VIDEO;
    }

    g_scene_state.scene_init = OT_TRUE;

    scene_logd("SCENE module has been inited successfully.\n");
err_exit:
    ot_mutex_unlock(g_scene_lock);
    return ret;
}

ot_s32 ot_scene_set_param(const ot_scene_param *scene_param)
{
    ot_scenecomm_check_pointer_return(scene_param, OT_SCENE_ENONPTR);

    ot_s32 ret = OT_FAILURE;

    ot_mutex_lock(g_scene_lock);
    if (g_scene_state.scene_init == OT_FALSE) {
        scene_loge("sceneauto not init yet!\n");
        ret = OT_SCENE_ENOTINIT;
        goto err_exit;
    }

    ret = ot_scene_set_pipe_param(scene_param->pipe_param, OT_SCENE_PIPETYPE_NUM);
    if (ret != OT_SUCCESS) {
        scene_loge("SCENE module has already been inited.\n");
        ret =  OT_SCENE_EINTER;
        goto err_exit;
    }

err_exit:
    ot_mutex_unlock(g_scene_lock);
    return ret;
}

ot_s32 ot_scene_deinit(ot_void)
{
    ot_mutex_lock(g_scene_lock);
    if (g_scene_state.scene_init == OT_FALSE) {
        scene_loge("sceneauto not init yet!\n");
        ot_mutex_unlock(g_scene_lock);
        return OT_SCENE_ENOTINIT;
    }

    g_scene_state.scene_init = OT_FALSE;

    scene_logd("SCENE Module has been deinited successfully!\n");
    ot_mutex_unlock(g_scene_lock);
    return OT_SUCCESS;
}

ot_s32 ot_scene_set_state(const ot_vi_pipe_handle vi_pipe_hdl[], ot_u32 pipe_num)
{
    ot_s32 ret = OT_FAILURE;
    ot_scenecomm_check_pointer_return(vi_pipe_hdl, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(pipe_num >= OT_SCENE_PIPE_MAX_NUM, OT_SCENE_EOUTOFRANGE);
    
    ot_mutex_lock(g_scene_lock);
    if (g_scene_state.scene_init == OT_FALSE) {
        scene_loge("sceneauto not init yet!\n");
        ret = OT_SCENE_ENOTINIT;
        goto err_exit;
    }

    for (int i = 0; i < pipe_num; i++) {
        ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
        ot_vi_pipe_id master_pipe_id = 0;
        ot_s32 pipe_num = 0;
        ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(vi_pipe_hdl[i], pipe_id, &pipe_num);
        master_pipe_id = pipe_id[0];
        ot_scenecomm_check_goto(ret, err_exit);

        g_scene_state.pipe_param_idx[master_pipe_id] = i;
    }
    
    ret = OT_SUCCESS;
err_exit:
    ot_mutex_unlock(g_scene_lock);
    return ret;
}

ot_s32 ot_scene_get_param_index(ot_vi_pipe_handle pipe_hdl, ot_s32 *index)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("clut set param failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];
    *index = g_scene_state.pipe_param_idx[master_pipe_id];
    return OT_SUCCESS;
}

ot_s32 ot_scene_pause(ot_vi_pipe_handle pipe_hdl, ot_bool enable)
{
    ot_s32 ret = OT_FAILURE;
    ot_mutex_lock(g_scene_lock);
    if (g_scene_state.scene_init == OT_FALSE) {
        scene_loge("scene auto not init yet!\n");
        ret =  OT_SCENE_ENOTINIT;
        goto err_exit;
    }

    ret = ot_scene_set_pause(pipe_hdl, enable);
    if (ret != OT_SUCCESS) {
        ret = OT_SCENE_EINTER;
        goto err_exit;
    }

err_exit:    
    ot_mutex_unlock(g_scene_lock);
    return ret;
}

ot_s32 ot_scene_dynamic_abnr_init(ot_vi_pipe_handle pipe_hdl, ot_u32 sns_type)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("abnr init failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    ot_eis_scene_abnr_attr abnr_attr = {0};

    // sensor
    if (sns_type == SONY_IMX623_MIPI_3M_60FPS_16BIT) {
        abnr_attr.sensor = OT_EIS_SCENE_SENSOR_IMX623;
    } else if (sns_type == OV_OX03C_MIPI_3M_60FPS_16BIT) {
        abnr_attr.sensor = OT_EIS_SCENE_SENSOR_OX03C10;
    } else if (sns_type == OV_OX03F10_MIPI_3M_60FPS_16BIT) {
        abnr_attr.sensor = OT_EIS_SCENE_SENSOR_OX03F10;
    }

    // thresh
    ot_u8 index = g_scene_state.pipe_param_idx[master_pipe_id];
    ot_scene_dynamic_abnr *dynamic_abnr = &get_pipe_params()[index].dynamic_abnr;
    abnr_attr.thresh_cnt = dynamic_abnr->abnr_thresh_cnt;
    for (int i = 0; i < OT_EIS_SCENE_ABNR_THR_SIZE; i++) {
        abnr_attr.thresh[i] = dynamic_abnr->abnr_thresh[i];
    }

    // roi
    ot_eis_img_size img_size = {0};
    sample_comm_vi_get_size_by_sns_type(sns_type, &img_size);
    abnr_attr.roi_rect.x = 0;
    abnr_attr.roi_rect.y = 0;
    abnr_attr.roi_rect.width = img_size.width;
    abnr_attr.roi_rect.height = img_size.height;

    ret = ot_eis_scene_abnr_init(pipe_hdl, &abnr_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;

}

ot_s32 ot_scene_dynamic_abnr_set_param(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("abnr set param failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    ot_u8 index = g_scene_state.pipe_param_idx[master_pipe_id];

    ret = ot_scene_set_dynamic_abnr(pipe_hdl, index);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_abnr_set_roi(ot_vi_pipe_handle pipe_hdl, ot_eis_img_point *roi)
{
    ot_scenecomm_check_pointer_return(roi, OT_SCENE_ENONPTR);
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("abnr set roi failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }

    ret = ot_eis_scene_abnr_set_roi(pipe_hdl, roi);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_abnr_deinit(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("abnr deinit failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }

    ret = ot_eis_scene_abnr_deinit(pipe_hdl);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_iqt_init(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_chn_hdl, const ot_scene_iqt_attr *attr, ot_eis_handle *task_hdl)
{
    ot_scenecomm_check_pointer_return(attr, OT_SCENE_ENONPTR);
    ot_scenecomm_check_pointer_return(task_hdl, OT_SCENE_ENONPTR);

    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("abnr set param failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    ot_u8 index = g_scene_state.pipe_param_idx[master_pipe_id];
    ot_eis_scene_iqt_attr dynamic_iqt = {0};
    
    memcpy_s(&dynamic_iqt, sizeof(dynamic_iqt), &get_pipe_params()[index].dynamic_iqt.iqt_attr, sizeof(get_pipe_params()[index].dynamic_iqt.iqt_attr));
    dynamic_iqt.input_attr.input_size.width = attr->width;
    dynamic_iqt.input_attr.input_size.height = attr->height;
    dynamic_iqt.input_attr.pixel_format = attr->pixel_format;
    dynamic_iqt.mdl_config.priority_preemp = OT_TRUE;
    dynamic_iqt.mdl_config.priority_level = OT_AVP_MDL_PRI_HIGH;
        
    ret = ot_eis_scene_iqt_init(pipe_hdl, vproc_chn_hdl, &dynamic_iqt, task_hdl);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_iqt_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_handle task_hdl, ot_eis_video_frame *video_frame)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("abnr set param failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    ot_u8 index = g_scene_state.pipe_param_idx[master_pipe_id];
    ret = ot_scene_set_dynamic_iqt(pipe_hdl, task_hdl, video_frame, index);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_iqt_deinit(ot_eis_handle task_hdl)
{
    ot_s32 ret = ot_eis_scene_iqt_deinit(task_hdl);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_mce_init(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("mce init failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }

    ret = ot_eis_scene_mce_init(pipe_hdl);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_mce_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_video_frame *video_frame)
{
    ot_scenecomm_check_pointer_return(video_frame, OT_SCENE_ENONPTR);
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("mce set param failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    ret = ot_scene_set_dynamic_mce(pipe_hdl, video_frame, g_scene_state.pipe_param_idx[master_pipe_id]);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_mce_deinit(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("mce deinit failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }

    ret = ot_eis_scene_mce_deinit(pipe_hdl);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_deflare_init(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("deflare init failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_deflare_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_video_frame *video_frame)
{
    ot_scenecomm_check_pointer_return(video_frame, OT_SCENE_ENONPTR);
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("deflare set param failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_deflare_deinit(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("deflare deinit failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_deflicker_init(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("deflicker init failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }
    master_pipe_id = pipe_id[0];

    ot_u32 index = g_scene_state.pipe_param_idx[master_pipe_id];
    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_SCENE_EOUTOFRANGE);

    ret = ot_eis_scene_deflicker_init(pipe_hdl, &(get_pipe_params()[index].static_ae.deflicker_attr));
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);
    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_deflicker_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_video_frame *video_frame)
{
    ot_scenecomm_check_pointer_return(video_frame, OT_SCENE_ENONPTR);
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("deflicker set param failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }

    ret = ot_eis_scene_deflicker_set_param(pipe_hdl, video_frame);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 ot_scene_dynamic_deflicker_deinit(ot_vi_pipe_handle pipe_hdl)
{
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_s32 pipe_num = 0;
    ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    if (ret != OT_SUCCESS) {
        scene_loge("deflicker deinit failed. invalid pipe_hdl\n");
        return OT_SCENE_INVALID_PIPE_HDL;
    }

    ret = ot_eis_scene_deflicker_deinit(pipe_hdl);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
