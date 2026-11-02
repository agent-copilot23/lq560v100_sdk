/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "sample_comm.h"
#include "securec.h"
#include "ot_buffer_pool.h"
#include "ot_eis_vi_api.h"

static volatile sig_atomic_t g_sig_flag = 0;

ot_eis_vi_vproc_mode_type g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;

static ot_void sample_get_char(ot_void)
{
    if (g_sig_flag == 1) {
        return;
    }

    sample_pause();
}

static ot_s32 sample_vi_user_mode_buf_pool_creat(sample_sns_type sns_type, ot_eis_handle pool_hdl)
{
    ot_s32 ret;
    ot_eis_img_size size;
    ot_u32 block_size;
    ot_video_buffer_attr pool_attr = { 0 };
    ot_eis_img_attr img_attr = { 0 };
    sample_comm_vi_get_size_by_sns_type(sns_type, &size);

    /* default YUV pool: SP420 + compress_none */
    img_attr.bit_depth = OT_EIS_PIXEL_BIT_DEPTH_8;
    img_attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE;
    img_attr.height = size.height;
    img_attr.width = size.width;
    img_attr.layout = OT_EIS_IMAGE_LAYOUT_LINEAR;
    img_attr.pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR;

    sample_common_get_buffer_pool_cfg(&img_attr, &block_size);
    pool_attr.cnt = 1;
    pool_attr.buf_blks[0].cnt = 10;
    pool_attr.buf_blks[0].size = block_size;

    ret = ot_buffer_pool_init();
    if(ret != 0){
       printf("buffer_pool init fail! ret[%d]\n", ret);
       return OT_FAILURE;
    }

    ret =  ot_buffer_pool_create(pool_hdl, &pool_attr);
    if(ret != 0){
       printf("buffer_pool create fail! ret[%d]\n", ret);
       goto pool_create_failed;
    }

    return OT_SUCCESS;

pool_create_failed:
    ot_buffer_pool_deinit();
    return ret;
}

static ot_void sample_vi_user_mode_buf_pool_destroy(ot_eis_handle pool_hdl)
{
    ot_buffer_pool_destroy(pool_hdl);
    ot_buffer_pool_deinit();
}

typedef struct {
    ot_eis_handle vi_chn_hdl;
    ot_eis_handle vo_inport_hdl;
} vi_chn_send_attr;
static ot_bool g_send_chn_pthread = OT_FALSE;

static ot_void *sample_vi_get_frame_send_to_vo_thread(ot_void *param)
{
    ot_s32 ret;
    ot_eis_handle vi_chn_hdl;
    ot_eis_handle vo_inport_hdl;
    vi_chn_send_attr *send_attr = (vi_chn_send_attr *)param;
    ot_eis_video_frame video_frame = { 0 };
    ot_s32 milli_sec = -1;

    vi_chn_hdl = send_attr->vi_chn_hdl;
    vo_inport_hdl = send_attr->vo_inport_hdl;
    while (g_send_chn_pthread) {
        ret = ot_eis_vi_chn_get_frame(vi_chn_hdl, &video_frame, milli_sec);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vi_chn_get_frame failed with %#x\n", ret);
            goto exit;
        }

        ret = ot_eis_vo_in_port_send_frame(vo_inport_hdl, &video_frame);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vo_in_port_send_frame failed with %#x\n", ret);
            ot_eis_vi_chn_release_frame(vi_chn_hdl, &video_frame);
            goto exit;
        }

        ret = ot_eis_vi_chn_release_frame(vi_chn_hdl, &video_frame);
        if (ret != OT_SUCCESS) {
            goto exit;
        }
    }

exit:
    return OT_NULL;
}


static ot_s32 sample_vi_get_frame_send_to_vo(sample_vi_cfg *vi_cfg, sample_vo_attr *vo_attr)
{
    ot_s32 ret;
    pthread_t thread_id = 0;
    vi_chn_send_attr send_attr = { 0 };
    send_attr.vi_chn_hdl = vi_cfg->pipe_info[0].chn_info.chn_hdl;
    send_attr.vo_inport_hdl = vo_attr->in_port_handle[0];

    g_send_chn_pthread = OT_TRUE;
    ret = pthread_create(&thread_id, OT_NULL, sample_vi_get_frame_send_to_vo_thread, (ot_void *)&send_attr);
    if (ret != OT_SUCCESS) {
        printf("vi create send frame thread failed!\n");
        g_send_chn_pthread = OT_FALSE;
        return OT_FAILURE;
    }

    printf("vi send frame thread running, print any key to exit!\n");
    getchar();

    g_send_chn_pthread = OT_FALSE;
    pthread_join(thread_id, OT_NULL);
    return ret;
}

static ot_s32 sample_vio_all_mode(sample_vo_attr *vo_attr)
{
    ot_s32 ret = OT_FAILURE;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    if (sample_comm_vio_start_route(&vi_cfg, vo_attr) != OT_SUCCESS) {
        return ret;
    }

    if (vo_attr->surface_enable[1] == OT_TRUE)
    {
        printf("begin to show overlay surface\n");
        sample_common_draw_cornerbox(vo_attr);
    }

    sample_get_char();

    sample_comm_vio_stop_route(&vi_cfg, vo_attr);
    return OT_SUCCESS;
}

static ot_s32 sample_vio_wdr(sample_vo_attr *vo_attr)
{
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    if (sample_comm_vio_start_route(&vi_cfg, vo_attr) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    sample_get_char();

    sample_comm_vio_stop_route(&vi_cfg, vo_attr);
    return OT_SUCCESS;
}

static ot_s32 sample_vio_chn_uesr_mode(sample_vo_attr *vo_attr)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg;
    ot_eis_img_size  img_size = { 0 };
    ot_eis_handle pool_hdl = OT_NULL;
    ot_video_buffer_attr vrb_cfg;
    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    sample_comm_vi_get_size_by_sns_type(sns_type, &img_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_common_get_vrb_cfg(3, img_size.width, img_size.height, &vrb_cfg);

    if (sample_comm_sys_init(&vrb_cfg) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    ret = sample_vi_user_mode_buf_pool_creat(sns_type, &pool_hdl);
    if (ret != OT_SUCCESS) {
        goto buf_pool_creat_failed;
    }

    vi_cfg.pipe_info[0].chn_info.chn_attr.work_mode = OT_EIS_VI_CHN_WORK_MODE_USER;
    vi_cfg.pipe_info[0].chn_info.chn_attr.frame_queue_depth = 8;
    vi_cfg.pipe_info[0].chn_info.chn_attr.pool_handle = pool_hdl;

    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != OT_SUCCESS) {
        goto start_vi_failed;
    }

    ret = sample_comm_start_vo(vo_attr);
    if (ret != OT_SUCCESS) {
        goto start_vo_failed;
    }

    ret = sample_vi_get_frame_send_to_vo(&vi_cfg, vo_attr);

    sample_comm_stop_vo(vo_attr);
start_vo_failed:
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_vi_user_mode_buf_pool_destroy(pool_hdl);
buf_pool_creat_failed:
    sample_comm_sys_exit();

    return ret;
}

static ot_s32 sample_vo_check_params(sample_param_comb* param_comb)
{
    // sample_vo only supports one sensor case
    if (param_comb->input_param > ONE_MIPI_ONE_SNS_MAX) {
        sample_print("sample_vo only supports one sensor case.\n");
        return OT_FAILURE;
    }
    if (param_comb->output_param >= TI941_TI948_TM070JDHP08_1280X768_VO_2_PORT) {
        if (param_comb->sample_index != 4) {
            sample_print("check one sensor two source input vo failed.\n");
            return OT_FAILURE;
        }
    }
    return OT_SUCCESS;
}

static ot_void sample_vio_usage(const char *prg_name)
{
    printf("usage : %s <index> -i <input_param> -o <output_param>\n", prg_name);
    printf("index:\n");
    printf("    (0) all mode route          :vi(Online) -> vo\n");
    printf("    (1) wdr route               :vi wdr(Online) -> vo\n");
    printf("    (2) user mode               :user input -> vo\n");
    printf("    (3) all mode route          :vi(Online) -> vo(video+graph)\n");
    printf("    (4) all mode route          :vi(Online) -> vo(2 in port)\n");
    sample_comm_args_help();
    printf("\neg: %s 4 -i 0 -o 4\n\n", prg_name);
}

static ot_void sample_vio_handle_sig(ot_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        g_sig_flag = 1;
    }
}

static ot_void sample_register_sig_handler(ot_void (*sig_handle)(ot_s32))
{
    struct sigaction sa;

    (ot_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handle;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, OT_NULL);
    sigaction(SIGTERM, &sa, OT_NULL);
}

static ot_s32 sample_vio_execute_case(ot_u32 case_index)
{
    ot_s32 ret;
    sample_vo_attr *vo_attr = OT_NULL;

    if (case_index == 4) {
        vo_attr = sample_comm_get_vo_attr(TWO_IN_PORT_MIPI_768);
    } else {
        vo_attr = sample_comm_args_get_vo_attr();
    }

    if (vo_attr == OT_NULL) {
        sample_print("Invalid vo attr.\n");
        return OT_FAILURE;
    }

    switch (case_index) {
        case 0: /* 0 all mode route */
            ret = sample_vio_all_mode(vo_attr);
            break;
        case 1: /* 1 wdr route */
            ret = sample_vio_wdr(vo_attr);
            break;
        case 2: /* 2 user mode */
            ret = sample_vio_chn_uesr_mode(vo_attr);
            break;
        case 3: /* 3 all mode route(vo+oge) */
            vo_attr->surface_enable[1] = OT_TRUE;
            vo_attr->surface[1] = OT_EIS_VO_GRAPHIC_SURFACE_0;
            vo_attr->surface_attr[1] = vo_attr->surface_attr[0];
            ret = sample_vio_all_mode(vo_attr);
            break;
        case 4: /* 4 all mode(one sensor + vo two in port) */
            ret = sample_vio_all_mode(vo_attr);
            break;
        default:
            ret = OT_FAILURE;
            break;
    }

    return ret;
}

ot_s32 main(ot_s32 argc, ot_char *argv[])
{
    ot_s32 ret = OT_FAILURE;
    ot_u32 index;
    sample_param_comb *param_comb = OT_NULL;

    if ((argc < 2) || (argc > 6)) { /* 2:arg num */
        sample_vio_usage(argv[0]);
        return ret;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_vio_usage(argv[0]);
        return ret;
    }

    if (strlen(argv[1]) > 2 || strlen(argv[1]) <= 0 || !check_digit(argv[1][0]) || /* 2:arg len */
        (strlen(argv[1]) == 2 && (!check_digit(argv[1][1]) || argv[1][0] == '0'))) { /* 2:arg len */
        sample_vio_usage(argv[0]);
        return ret;
    }

    if (strlen(argv[1]) == 2 && argv[1][1] != '0') { /* 2:arg len, max: 10 */
        sample_vio_usage(argv[0]);
        return ret;
    }

    sample_register_sig_handler(sample_vio_handle_sig);

    index = strtol(argv[1], NULL, 10); /* base 10 */
    sample_comm_args_set_sample_index(index);

    ret = sample_comm_args_register(argc, argv);
    if (ret != OT_SUCCESS) {
        sample_vio_usage(argv[0]);
        return OT_FAILURE;
    }

    param_comb = sample_comm_args_get_param_comb();
    ret = sample_vo_check_params(param_comb);
    if (ret != OT_SUCCESS) {
        sample_vio_usage(argv[0]);
        return OT_FAILURE;
    }

    ret = sample_vio_execute_case(index);
    if ((ret == OT_SUCCESS) && (g_sig_flag == 0)) {
        printf("\033[0;32mprogram exit normally!\033[0;39m\n");
    } else {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    return ret;
}
