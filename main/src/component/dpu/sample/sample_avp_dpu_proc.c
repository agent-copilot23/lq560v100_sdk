/*
  Copyright (c), 2001-2024, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <limits.h>
#include "sample_comm.h"
#include "sample_avp_dpu_proc.h"
#include "sample_common_avp_dpu.h"

static ot_sample_avp_dpu_cfg g_dpu_cfg;
static ot_eis_handle g_pipe_file_hdl;
static ot_bool g_stop_signal = OT_FALSE;

static ot_void sample_avp_dpu_uninit_dpu_rect(ot_void)
{
    ot_u32 i;

    (ot_void)sample_common_avp_dpu_rect_stop(g_dpu_cfg.rect_grp_hdl, g_dpu_cfg.rect_chn_hdl,
        g_dpu_cfg.dpu_rect_grp_attr.rect_mode);
        for (i = 0; i < OT_AVP_DPU_RECT_MAX_PIPE_NUM; i++) {
            (ot_void)sample_common_avp_dpu_rect_unload_lut(&g_dpu_cfg.dpu_rect_mem_info[i], g_dpu_cfg.lut_id[i]);
    }
    return;
}

static ot_s32 sample_avp_dpu_start_dpu_rect(const ot_eis_img_size *src_size, const ot_eis_img_size *dst_size,
    ot_char *lut_file_name[], ot_u32 file_num)
{
    ot_s32 ret;
    ot_u32 i;
    ot_u32 rect_pipe_num = 0;
    ot_avp_dpu_rect_chn_attr rect_chn_attr[OT_AVP_DPU_RECT_MAX_CHN_NUM] = {0};
    ot_char path[AVP_DPU_PATH_MAX + 1] = {0};

    for (i = 0; i < file_num; i++) {
        sample_avp_dpu_check_exps_return(((strlen(lut_file_name[i]) > AVP_DPU_PATH_MAX) ||
            (realpath(lut_file_name[i], path) == OT_NULL)),
            OT_FAILURE, "Error, file_name is invalid!\n");
        ret = sample_common_avp_dpu_rect_load_lut(lut_file_name[i],
            &g_dpu_cfg.dpu_rect_mem_info[i], &g_dpu_cfg.lut_id[i]);
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail_0, "load lut failed!\n");
        rect_pipe_num++;
    }

    g_dpu_cfg.dpu_rect_grp_attr.left_attr.lut_id = g_dpu_cfg.lut_id[0];
    g_dpu_cfg.dpu_rect_grp_attr.right_attr.lut_id = g_dpu_cfg.lut_id[1];
    g_dpu_cfg.rect_grp_hdl = OT_NULL;
    g_dpu_cfg.dpu_rect_grp_attr.left_attr.image_size.width = src_size->width;
    g_dpu_cfg.dpu_rect_grp_attr.left_attr.image_size.height = src_size->height;
    g_dpu_cfg.dpu_rect_grp_attr.right_attr.image_size.width = src_size->width;
    g_dpu_cfg.dpu_rect_grp_attr.right_attr.image_size.height = src_size->height;
    g_dpu_cfg.dpu_rect_grp_attr.rect_mode = OT_AVP_DPU_RECT_MODE_DOUBLE;
    g_dpu_cfg.dpu_rect_grp_attr.depth = 1;
    g_dpu_cfg.dpu_rect_grp_attr.is_need_src_frame = OT_TRUE;
    g_dpu_cfg.dpu_rect_grp_attr.frame_rate.src_frame_rate = SAMPLE_AVP_DPU_NO_RATE_CTRL;
    g_dpu_cfg.dpu_rect_grp_attr.frame_rate.dst_frame_rate = SAMPLE_AVP_DPU_NO_RATE_CTRL;

    for (i = 0; i < OT_AVP_DPU_RECT_MAX_CHN_NUM; i++) {
        rect_chn_attr[i].image_size.width = dst_size->width;
        rect_chn_attr[i].image_size.height = dst_size->height;
        g_dpu_cfg.rect_chn_hdl[i] = OT_NULL;
    }

    ret = sample_common_avp_dpu_rect_start(&g_dpu_cfg.rect_grp_hdl,
        &g_dpu_cfg.dpu_rect_grp_attr, rect_chn_attr, g_dpu_cfg.rect_chn_hdl);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail_1, "start dpu rect failed!\n");

    return OT_SUCCESS;
fail_1:
    (ot_void)sample_common_avp_dpu_rect_stop(g_dpu_cfg.rect_grp_hdl, g_dpu_cfg.rect_chn_hdl, g_dpu_cfg.dpu_rect_grp_attr.rect_mode);
fail_0:
    for (i = 0; i < rect_pipe_num; i++) {
        (ot_void)sample_common_avp_dpu_rect_unload_lut(&g_dpu_cfg.dpu_rect_mem_info[i], g_dpu_cfg.lut_id[i]);
    }
    return ret;
}

static ot_void sample_avp_dpu_uninit_dpu_match(ot_void)
{
    (ot_void)sample_common_avp_dpu_match_stop(g_dpu_cfg.match_grp_hdl, g_dpu_cfg.match_chn_hdl);
    sample_avp_dpu_smr_free(g_dpu_cfg.dpu_match_grp_attr.assist_buf.phys_addr);
    return;
}

static ot_s32 sample_avp_dpu_set_match_grp_common_attr(ot_u32 width, ot_u32 height)
{
    ot_s32 ret;
    ot_u32 size;

    ret = ot_avp_dpu_match_get_assist_buf_size(SAMPLE_AVP_DPU_DISPARITY_NUM, height, &size);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "get dpu match assist buffer size failed!\n");
    ret = sample_common_avp_dpu_match_create_mem_info(&g_dpu_cfg.dpu_match_grp_attr.assist_buf,
        "sampe_dpu_match_assist_buf", OT_NULL, size);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "create dpu match assist buffer failed!\n");
    g_dpu_cfg.match_grp_hdl = OT_NULL;
    g_dpu_cfg.match_chn_hdl[0] = OT_NULL;
    g_dpu_cfg.dpu_match_grp_attr.left_image_size.width = width;
    g_dpu_cfg.dpu_match_grp_attr.left_image_size.height = height;
    g_dpu_cfg.dpu_match_grp_attr.right_image_size.width = width;
    g_dpu_cfg.dpu_match_grp_attr.right_image_size.height = height;
    g_dpu_cfg.dpu_match_grp_attr.invalid_disp = SAMPLE_AVP_DPU_INVALID_DISP;
    g_dpu_cfg.dpu_match_grp_attr.depth = 1;
    g_dpu_cfg.dpu_match_grp_attr.is_need_src_frame = OT_TRUE;
    g_dpu_cfg.dpu_match_grp_attr.frame_rate.src_frame_rate = SAMPLE_AVP_DPU_NO_RATE_CTRL;
    g_dpu_cfg.dpu_match_grp_attr.frame_rate.dst_frame_rate = SAMPLE_AVP_DPU_NO_RATE_CTRL;
    return OT_SUCCESS;
}

static ot_s32 sample_avp_dpu_set_match_grp_disp_attr(ot_void)
{
    ot_s32 ret;

    g_dpu_cfg.dpu_match_grp_attr.disp_param.version = OT_AVP_DPU_MATCH_DISP_VERSION_V1;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.is_proc_disp = OT_TRUE;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.is_output_disp = OT_TRUE;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.is_output_conf = OT_TRUE;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.is_post_proc = OT_TRUE;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.is_filter_disp_by_conf = OT_TRUE;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.is_time_domain_filter = OT_TRUE;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.is_enable_disp_subpixel = OT_TRUE;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.mask_mode = OT_AVP_DPU_MATCH_MASK_9X9_MODE;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.speed_accuracy_mode = OT_AVP_DPU_MATCH_SPEED_ACCURACY_MODE_ACCURACY;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.bg_mode = OT_AVP_DPU_MATCH_BG_MODE_NONE;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.opti_mode = OT_AVP_DPU_MATCH_OPTI_MODE_ADAPTIVE;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.disp_num = SAMPLE_AVP_DPU_DISPARITY_NUM;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.disp_start_pos = 0;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.first_penalty_coef = SAMPLE_AVP_DPU_MATCH_FIRST_PENALTY_COEF;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.second_penalty_coef = SAMPLE_AVP_DPU_MATCH_SECOND_PENALTY_COEF;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.fg_init_cost_zero_thr = SAMPLE_AVP_DPU_MATCH_FG_INIT_COST_ZERO_THR;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.local_feature = SAMPLE_AVP_DPU_MATCH_LOCAL_FAETURE;
    g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.conf_filter_disp_thr = SAMPLE_AVP_DPU_MATCH_CONF_FILTER_DISP_THR;
    ret = sample_common_avp_dpu_get_fg_aggregate_coef_and_unique_ratio(
        SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D11_A0,
        &g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.aggregate_coef,
        &g_dpu_cfg.dpu_match_grp_attr.disp_param.v1.unique_ratio);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "get fg aggregate coef failed!\n");
    return OT_SUCCESS;
}

static ot_void sample_avp_dpu_set_match_grp_depth_attr(ot_void)
{
    g_dpu_cfg.dpu_match_grp_attr.depth_param.version = OT_AVP_DPU_MATCH_DEPTH_VERSION_V1;
    g_dpu_cfg.dpu_match_grp_attr.depth_param.v1.is_output_depth = OT_TRUE;
    g_dpu_cfg.dpu_match_grp_attr.depth_param.v1.disp_from = OT_AVP_DPU_MATCH_DISP_FROM_MATCH;
    g_dpu_cfg.dpu_match_grp_attr.depth_param.v1.fb_norm_bits = SAMPLE_AVP_DPU_MATCH_FB_NORM_BITS_8;
    g_dpu_cfg.dpu_match_grp_attr.depth_param.v1.fb = SAMPLE_AVP_DPU_MATCH_FB;
    g_dpu_cfg.dpu_match_grp_attr.depth_param.v1.z0 = SAMPLE_AVP_DPU_MATCH_Z0;
    g_dpu_cfg.dpu_match_grp_attr.depth_param.v1.min_depth_thr = SAMPLE_AVP_DPU_MATCH_MIN_DEPTH_THR;
    g_dpu_cfg.dpu_match_grp_attr.depth_param.v1.max_depth_thr = SAMPLE_AVP_DPU_MATCH_MAX_DEPTH_THR;
    g_dpu_cfg.dpu_match_grp_attr.depth_param.v1.invalid_depth = SAMPLE_AVP_DPU_MATCH_INVALID_DEPTH;
    g_dpu_cfg.dpu_match_grp_attr.depth_param.v1.disp_coef = SAMPLE_AVP_DPU_MATCH_DISP_COEF;
    g_dpu_cfg.dpu_match_grp_attr.depth_param.v1.depth_prec = g_dpu_cfg.dpu_match_grp_attr.depth_param.v1.fb_norm_bits -
        SAMPLE_AVP_DPU_MATCH_FB_NORM_BITS_8;
}
static ot_s32 sample_avp_dpu_start_dpu_match(ot_u32 width, ot_u32 height)
{
    ot_s32 ret;
    ot_avp_dpu_match_chn_attr match_chn_attr = {0};
    ret = sample_avp_dpu_set_match_grp_common_attr(width, height);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "set match grp common attr failed!\n");

    ret = sample_avp_dpu_set_match_grp_disp_attr();
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "set match grp disp attr failed!\n");
    sample_avp_dpu_set_match_grp_depth_attr();

    match_chn_attr.image_size.width = width;
    match_chn_attr.image_size.height = height;
    ret = sample_common_avp_dpu_match_start(&g_dpu_cfg.match_grp_hdl, g_dpu_cfg.match_chn_hdl, &g_dpu_cfg.dpu_match_grp_attr, &match_chn_attr);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "dpu match start failed!\n");
    return OT_SUCCESS;
fail:
    (ot_void)sample_common_avp_dpu_match_stop(g_dpu_cfg.match_grp_hdl, g_dpu_cfg.match_chn_hdl);
    sample_avp_dpu_smr_free(g_dpu_cfg.dpu_match_grp_attr.assist_buf.phys_addr);
    return ret;
}

static ot_s32 sample_avp_dpu_create_pool(const ot_eis_img_size *pic_size)
{
    ot_s32 ret;
    ot_u32 i;
    ot_u32 stride;
    ot_s32 size;
    ot_video_buffer_attr vrb_pool_cfg;

    stride = sample_common_avp_dpu_calc_stride(pic_size->width, SAMPLE_AVP_DPU_ALIGN_16);
    size = stride * pic_size->height;
    for (i = 0; i < OT_AVP_DPU_RECT_MAX_PIPE_NUM; i++) {
        ret = memset_s(&vrb_pool_cfg, sizeof(ot_video_buffer_attr), 0, sizeof(ot_video_buffer_attr));
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail_0, "memset_s for vrb_pool_cfg failed!\n");

        vrb_pool_cfg.cnt  = 1;
        vrb_pool_cfg.buf_blks[0].size = size;
        vrb_pool_cfg.buf_blks[0].cnt   = 1;
        strcpy(vrb_pool_cfg.buf_blks[0].region_name, "anony");
        ret = ot_buffer_pool_create(&g_dpu_cfg.pipe_vrb_pool[i], &vrb_pool_cfg);
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS,
            fail_0, "create vrb pool failed!\n");

        ret = ot_buffer_pool_blk_acquire(g_dpu_cfg.pipe_vrb_pool[i], size, &g_dpu_cfg.vrb_blk[i]);
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS,
            fail_1, "ot_buffer_pool_blk_acquire failed!\n");
    }
    return OT_SUCCESS;
fail_1:
    for (i = 0; i < OT_AVP_DPU_RECT_MAX_PIPE_NUM; i++) {
        ret = ot_buffer_pool_blk_release(g_dpu_cfg.pipe_vrb_pool[i], &g_dpu_cfg.vrb_blk[i]);
        g_dpu_cfg.vrb_blk[i].phys_addr = (ot_phys_addr)OT_NULL;
        g_dpu_cfg.vrb_blk[i].block_size = 0;
    }
fail_0:
    for (i = 0; i < OT_AVP_DPU_RECT_MAX_PIPE_NUM; i++) {
        if (g_dpu_cfg.pipe_vrb_pool[i] != OT_NULL) {
            (ot_void)ot_buffer_pool_destroy(g_dpu_cfg.pipe_vrb_pool[i]);
        }
    }
    return ret;
}

static ot_s32 sample_avp_dpu_sys_init_file_case(ot_eis_img_size *pic_size, ot_u32 pic_size_num)
{
    ot_s32 ret;
    ot_pic_size pic_size_type = PIC_640X480;
    ot_video_buffer_attr vrb_cfg;
    g_stop_signal = OT_FALSE;

    ret = memset_s(&g_dpu_cfg, sizeof(ot_sample_avp_dpu_cfg), 0, sizeof(ot_sample_avp_dpu_cfg));
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, OT_FAILURE,
        "memset_s for dpu_cfg failed!\n");

    sample_avp_dpu_check_exps_return(pic_size_num < SAMPLE_AVP_DPU_CHN_NUM, OT_FAILURE, "pic_size_num is illegal!\n");
    ret = sample_comm_sys_get_pic_size(pic_size_type, &pic_size[0]);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "sys get pic size failed!\n");

    ret = sample_comm_sys_get_pic_size(pic_size_type, &pic_size[1]);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "sys get pic size failed!\n");

    ret = memset_s(&vrb_cfg, sizeof(ot_video_buffer_attr), 0, sizeof(ot_video_buffer_attr));
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "memset_s for vrb_cfg failed!\n");

    sample_common_get_vrb_cfg(3, pic_size[0].width, pic_size[0].height, &vrb_cfg);

    ret = sample_comm_sys_init(&vrb_cfg);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "init sys failed!\n");

    ret = ot_buffer_pool_init();
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "init buffer_pool failed!\n");

    ret = sample_avp_dpu_create_pool(&pic_size[0]);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "sample_avp_dpu_create_pool failed!\n");

    ret = sample_comm_media_pipe_init(&g_pipe_file_hdl);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "sample_comm_media_pipe_init failed!\n");
    return ret;

fail:
    ot_buffer_pool_deinit();
    sample_comm_sys_exit();
    return ret;
}

static ot_void sample_avp_dpu_sys_uninit_file_case(ot_void)
{
    ot_u32 i;

    sample_comm_media_pipe_stop(g_pipe_file_hdl);

    for (i = 0; i < OT_AVP_DPU_RECT_MAX_PIPE_NUM; i++) {
        (ot_void)ot_buffer_pool_blk_release(g_dpu_cfg.pipe_vrb_pool, &g_dpu_cfg.vrb_blk[i]);
        g_dpu_cfg.vrb_blk[i].block_size = 0;
        g_dpu_cfg.vrb_blk[i].phys_addr  = (ot_phys_addr)OT_NULL;
    }
    for (i = 0; i < OT_AVP_DPU_RECT_MAX_PIPE_NUM; i++) {
        if (g_dpu_cfg.pipe_vrb_pool[i] != OT_NULL) {
            (ot_void)ot_buffer_pool_destroy(g_dpu_cfg.pipe_vrb_pool[i]);
        }
    }
    ot_buffer_pool_deinit();
    sample_comm_sys_exit();
    return;
}

static ot_s32 sample_avp_dpu_open_file(ot_char *src_file_name[OT_AVP_DPU_RECT_MAX_PIPE_NUM], ot_char *rect_out_file_name[OT_AVP_DPU_RECT_MAX_PIPE_NUM],
    ot_u32 src_file_num, const ot_char *dst_file_name[SAMPLE_AVP_DPU_OUTPUT_NUM], ot_u32 dst_file_num)
{
    ot_u32 i;
    ot_char path[AVP_DPU_PATH_MAX + 1] = {0};
    sample_avp_dpu_check_exps_return((src_file_num > OT_AVP_DPU_RECT_MAX_PIPE_NUM) || (src_file_num == 0),
        OT_FAILURE, "Error, src_file_num(%u) must be (%u, %u]!\n", src_file_num, 0,
        OT_AVP_DPU_RECT_MAX_PIPE_NUM);
    sample_avp_dpu_check_exps_return((dst_file_num > SAMPLE_AVP_DPU_OUTPUT_NUM) || (dst_file_num == 0),
        OT_FAILURE, "Error, dst_file_num(%u) must be (%u, %u]!\n", dst_file_num, 0,
        SAMPLE_AVP_DPU_OUTPUT_NUM);

    for (i = 0; i < src_file_num; i++) {
        sample_avp_dpu_check_exps_return((strlen(src_file_name[i]) > AVP_DPU_PATH_MAX) ||
            (realpath(src_file_name[i], path) == OT_NULL),
            OT_FAILURE, "Error, file_name is invalid!\n");
        g_dpu_cfg.pipe_file[i] = fopen(path, "rb");
        sample_avp_dpu_check_exps_goto(g_dpu_cfg.pipe_file[i] == OT_NULL,
            fail, "fopen file %s failed!\n", src_file_name[i]);
    }

    for (i = 0; i < src_file_num; i++) {
        g_dpu_cfg.rect_out_file[i] = fopen(rect_out_file_name[i], "wb");
        sample_avp_dpu_check_exps_goto(g_dpu_cfg.rect_out_file[i] == OT_NULL,
            fail, "fopen rect_out_file %s failed!\n", rect_out_file_name[i]);
    }

    for (i = 0; i < dst_file_num; i++) {
        g_dpu_cfg.dst_file[i] = fopen(dst_file_name[i], "wb");
        sample_avp_dpu_check_exps_goto(g_dpu_cfg.dst_file[i] == OT_NULL,
            fail, "fopen file %s failed!\n", dst_file_name[i]);
    }

    return OT_SUCCESS;
fail:
    for (i = 0; i < dst_file_num; i++) {
        if (g_dpu_cfg.dst_file[i] != OT_NULL) {
            fclose(g_dpu_cfg.dst_file[i]);
            g_dpu_cfg.dst_file[i] = OT_NULL;
        }
    }

    for (i = 0; i < src_file_num; i++) {
        if (g_dpu_cfg.pipe_file[i] != OT_NULL) {
            fclose(g_dpu_cfg.pipe_file[i]);
            g_dpu_cfg.pipe_file[i] = OT_NULL;
        }
    }

    return OT_FAILURE;
}

static void sample_avp_dpu_close_file(ot_void)
{
    ot_u32 i;
    for (i = 0; i < SAMPLE_AVP_DPU_OUTPUT_NUM; i++) {
        if (g_dpu_cfg.dst_file[i] != OT_NULL) {
            fclose(g_dpu_cfg.dst_file[i]);
            g_dpu_cfg.dst_file[i] = OT_NULL;
        }
    }

    for (i = 0; i < OT_AVP_DPU_RECT_MAX_PIPE_NUM; i++) {
        if (g_dpu_cfg.pipe_file[i] != OT_NULL) {
            fclose(g_dpu_cfg.pipe_file[i]);
            g_dpu_cfg.pipe_file[i] = OT_NULL;
        }
    }
    return;
}

static ot_void sample_avp_dpu_file_rect_match_stop(ot_void)
{
    ot_u32 i;
    sample_avp_dpu_close_file();

    (ot_void)sample_common_avp_dpu_rect_unbind_match(g_pipe_file_hdl, g_dpu_cfg.rect_chn_hdl[0], g_dpu_cfg.match_chn_hdl[0]);
    (ot_void)sample_common_avp_dpu_match_stop(g_dpu_cfg.match_grp_hdl, g_dpu_cfg.match_chn_hdl);
    sample_avp_dpu_smr_free(g_dpu_cfg.dpu_match_grp_attr.assist_buf.phys_addr);
    (ot_void)sample_common_avp_dpu_rect_stop(g_dpu_cfg.rect_grp_hdl, g_dpu_cfg.rect_chn_hdl, g_dpu_cfg.dpu_rect_grp_attr.rect_mode);

    for (i = 0; i < OT_AVP_DPU_RECT_MAX_PIPE_NUM; i++) {
        (ot_void)sample_common_avp_dpu_rect_unload_lut(&g_dpu_cfg.dpu_rect_mem_info[i], g_dpu_cfg.lut_id[i]);
    }

    for (i = 0; i < OT_AVP_DPU_RECT_MAX_PIPE_NUM; i++) {
        (ot_void)ot_buffer_pool_blk_release(g_dpu_cfg.pipe_vrb_pool, &g_dpu_cfg.vrb_blk[i]);
        g_dpu_cfg.vrb_blk[i].block_size = 0;
        g_dpu_cfg.vrb_blk[i].phys_addr  = (ot_phys_addr)OT_NULL;
    }

    for (i = 0; i < OT_AVP_DPU_RECT_MAX_PIPE_NUM; i++) {
        if (g_dpu_cfg.pipe_vrb_pool[i] != OT_NULL) {
            (ot_void)ot_buffer_pool_destroy(g_dpu_cfg.pipe_vrb_pool[i]);
        }
    }
    g_stop_signal = OT_FALSE;
    ot_buffer_pool_deinit();
    sample_comm_sys_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    return;
}

static ot_s32 sample_avp_dpu_proc_frame_from_file(const ot_eis_img_size *pic_size)
{
    ot_s32 ret;
    ot_u32 stride;
    ot_u32 i;
    ot_u32 frame_num;
    ot_s32 milli_sec = SAMPLE_AVP_DPU_TIME_OUT;
    ot_eis_img_frame dpu_rect_src_frame[OT_AVP_DPU_RECT_MAX_PIPE_NUM] = {0};
    ot_avp_dpu_rect_frame_info rect_out_frame = {0};

    stride = sample_common_avp_dpu_calc_stride(pic_size->width, SAMPLE_AVP_DPU_ALIGN_16);
    for (frame_num = 0; frame_num < SAMPLE_AVP_DPU_FRAME_NUM && g_stop_signal == OT_FALSE; frame_num++) {
        for (i = 0; i < OT_AVP_DPU_RECT_MAX_PIPE_NUM; i++) {
            ret = sample_common_avp_dpu_rect_get_frame_from_file(g_dpu_cfg.pipe_file[i], pic_size,
                stride, &dpu_rect_src_frame[i], g_dpu_cfg.vrb_blk[i]);
            sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "get frame failed!\n");
        }

        ret = ot_avp_dpu_rect_send_frame(g_dpu_cfg.rect_grp_hdl, &dpu_rect_src_frame[0],
            &dpu_rect_src_frame[1], milli_sec);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "ot_avp_dpu_rect_send_frame failed!\n");

        //rect get_frame and write file
        ret = ot_avp_dpu_rect_get_frame(g_dpu_cfg.rect_grp_hdl, SAMPLE_AVP_DPU_BLOCK, &rect_out_frame);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "ot_avp_dpu_rect_get_frame failed!\n");

        ret = sample_common_avp_dpu_write_frame_to_file(g_dpu_cfg.rect_out_file[SAMPLE_AVP_DPU_RECT_OUT_FRAME_LEFT_IDX], sizeof(ot_u8), &rect_out_frame.rect_frame[SAMPLE_AVP_DPU_RECT_OUT_FRAME_LEFT_IDX]);
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "sample_common_avp_dpu_write_frame_to_file rect_out_file failed!\n");

        ret = sample_common_avp_dpu_write_frame_to_file(g_dpu_cfg.rect_out_file[SAMPLE_AVP_DPU_RECT_OUT_FRAME_RIGHT_IDX], sizeof(ot_u8), &rect_out_frame.rect_frame[SAMPLE_AVP_DPU_RECT_OUT_FRAME_RIGHT_IDX]);
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "sample_common_avp_dpu_write_frame_to_file rect_out_file failed!\n");

        ret = ot_avp_dpu_rect_release_frame(g_dpu_cfg.rect_grp_hdl, &rect_out_frame);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "ot_avp_dpu_rect_release_frame failed!\n");

       /* get match frame from DPU MATCH */
        milli_sec = SAMPLE_AVP_DPU_BLOCK;
        ret = ot_avp_dpu_match_get_frame(g_dpu_cfg.match_grp_hdl, milli_sec,
            &g_dpu_cfg.dpu_match_src_frame, &g_dpu_cfg.dpu_match_dst_frame);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "get dpu match frame failed!\n");

        /* use dst frame to do something */
        ret = sample_common_avp_dpu_write_frame_to_file(g_dpu_cfg.dst_file[SAMPLE_AVP_DPU_OUTPUT_DISP_IDX],
            sizeof(ot_u16), &g_dpu_cfg.dpu_match_dst_frame.disp_frame);
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "sample_common_avp_dpu_write_frame_to_file failed!\n");
        ret = sample_common_avp_dpu_write_frame_to_file(g_dpu_cfg.dst_file[SAMPLE_AVP_DPU_OUTPUT_CONF_IDX],
            sizeof(ot_u8), &g_dpu_cfg.dpu_match_dst_frame.conf_frame);
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "sample_common_avp_dpu_write_frame_to_file failed!\n");
        ret = sample_common_avp_dpu_write_frame_to_file(g_dpu_cfg.dst_file[SAMPLE_AVP_DPU_OUTPUT_DEPTH_IDX],
            sizeof(ot_u16), &g_dpu_cfg.dpu_match_dst_frame.depth_frame);
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "sample_common_avp_dpu_write_frame_to_file failed!\n");

        ret = ot_avp_dpu_match_release_frame(g_dpu_cfg.match_grp_hdl,
            &g_dpu_cfg.dpu_match_src_frame, &g_dpu_cfg.dpu_match_dst_frame);
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "release dpu match frame failed!\n");
    }
    sample_avp_dpu_trace_info("Process success!\n");
    return OT_SUCCESS;
fail:
    (ot_void)ot_avp_dpu_match_release_frame(g_dpu_cfg.match_grp_hdl,
        &g_dpu_cfg.dpu_match_src_frame, &g_dpu_cfg.dpu_match_dst_frame);
    return ret;
}

/* This case only for function design reference */
ot_s32 sample_avp_dpu_file_rect_match(ot_void)
{
    ot_s32 ret;
    ot_eis_img_size pic_size[SAMPLE_AVP_DPU_CHN_NUM];
    ot_char *lut_file_name[OT_AVP_DPU_RECT_MAX_PIPE_NUM] = {
        "./data/input/lut/map_int_left_test.bin", "./data/input/lut/map_int_right_test.bin"
    };
    ot_char *src_file_name[OT_AVP_DPU_RECT_MAX_PIPE_NUM] = {
        "./data/input/src/000_640_480_l.yuv", "./data/input/src/000_640_480_r.yuv"
    };

    ot_char *rect_out_file_name[OT_AVP_DPU_RECT_MAX_PIPE_NUM] = {
        "./data/output/rect_000_640_480_l.yuv", "./data/output/rect_000_640_480_r.yuv"
    };

    const ot_char *dst_file_name[SAMPLE_AVP_DPU_OUTPUT_NUM] = { "./data/output/640_480_s16c1_disp.yuv",
        "./data/output/640_480_sp400_conf.yuv",
        "./data/output/640_480_u16c1_depth.yuv" };

    /*  init SYS, init common VRB(for DPU RECT and DPU MATCH) */
    ret = sample_avp_dpu_sys_init_file_case(pic_size, SAMPLE_AVP_DPU_CHN_NUM);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret, "sys_init in file case failed!\n");

    if (g_stop_signal == OT_TRUE) {
        sample_avp_dpu_file_rect_match_stop();
        return OT_FAILURE;
    }

    /*  start DPU RECT */
    ret = sample_avp_dpu_start_dpu_rect(&pic_size[0], &pic_size[1], lut_file_name, OT_AVP_DPU_RECT_MAX_PIPE_NUM);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail_0, "start dpu rect failed!\n");

    if (g_stop_signal == OT_TRUE) {
        sample_avp_dpu_file_rect_match_stop();
        return OT_FAILURE;
    }

    ret = sample_avp_dpu_start_dpu_match(pic_size[1].width, pic_size[1].height);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail_1, "dpu match start failed!\n");

    if (g_stop_signal == OT_TRUE) {
        sample_avp_dpu_file_rect_match_stop();
        return OT_FAILURE;
    }

    /* bind DPU RECT to DPU MATCH */
    ret = sample_common_avp_dpu_rect_bind_match(g_pipe_file_hdl, g_dpu_cfg.rect_chn_hdl[0], g_dpu_cfg.match_chn_hdl[0]);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail_2, "dpu rect bind match failed!\n");

    /* step5:  send frame to DPU RECT */
    ret = sample_avp_dpu_open_file(src_file_name, rect_out_file_name, OT_AVP_DPU_RECT_MAX_PIPE_NUM, dst_file_name, SAMPLE_AVP_DPU_OUTPUT_NUM);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail_3, "fopen file failed!\n");

    ret = sample_avp_dpu_proc_frame_from_file(&pic_size[0]);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail_3, "release dpu match frame failed!\n");

fail_3:
    sample_avp_dpu_close_file();
    (ot_void)sample_common_avp_dpu_rect_unbind_match(g_pipe_file_hdl, g_dpu_cfg.rect_chn_hdl[0], g_dpu_cfg.match_chn_hdl[0]);
fail_2:
    sample_avp_dpu_uninit_dpu_match();
fail_1:
    sample_avp_dpu_uninit_dpu_rect();
fail_0:
    sample_avp_dpu_sys_uninit_file_case();
    return ret;
}

ot_void sample_avp_dpu_file_rect_match_handle_sig(ot_void)
{
    g_stop_signal = OT_TRUE;
}

/* This case only for function design reference */
ot_s32 sample_avp_dpu_vi_vproc_rect_match(ot_void)
{
    return OT_SUCCESS;
}

ot_void sample_avp_dpu_vi_vproc_rect_match_handle_sig(ot_void)
{
    return;
}
