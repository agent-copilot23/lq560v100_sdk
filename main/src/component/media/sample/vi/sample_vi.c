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
#include "ot_board_info.h"

static volatile sig_atomic_t g_sig_flag = 0;
static ot_eis_vi_vproc_mode_type g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;

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
    if (ret != 0) {
        printf("buffer_pool init fail! ret[%d]\n", ret);
        return OT_FAILURE;
    }

    ret = ot_buffer_pool_create(pool_hdl, &pool_attr);
    if (ret != 0) {
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

static ot_void sample_vi_enable_low_delay(ot_vi_pipe_handle pipe_hdl, ot_vi_chn_handle chn_hdl, ot_eis_handle vproc_chn_hdl)
{
    ot_s32 ret;
    ot_eis_low_delay_info low_delay_info;

    low_delay_info.enable = OT_TRUE;
    low_delay_info.line_cnt = 200;
    low_delay_info.one_buf_en = OT_FALSE;

    ret = ot_eis_vi_pipe_set_low_delay_attr(pipe_hdl, &low_delay_info);
    if (ret != OT_SUCCESS) {
        sample_print("enable vi pipe low delay failed!\n");
    }

    ret = ot_eis_vi_chn_set_low_delay_attr(chn_hdl, &low_delay_info);
    if (ret != OT_SUCCESS) {
        sample_print("enable vi chn low delay failed!\n");
    }

    ret = ot_eis_vproc_chn_set_low_delay(vproc_chn_hdl, &low_delay_info);
    if (ret != OT_SUCCESS) {
        sample_print("enable vproc chn low delay failed!\n");
    }
}

static ot_void sample_vio_print_vi_mode_list()
{
    printf("vi vproc mode list: \n");
    printf("    (0) VI_ONLINE_VPROC_ONLINE\n");
    printf("    (1) VI_ONLINE_VPROC_OFFLINE\n");
    printf("    (2) VI_OFFLINE_VPROC_OFFLINE\n");
    printf("please select mode:\n");
}

static ot_void sample_vio_get_vi_vproc_mode_by_char(ot_char ch, ot_bool is_wdr)
{
    switch (ch) {
        case '0':
            g_mode_type = OT_EIS_VI_ONLINE_VPROC_ONLINE;
            break;
        case '1':
            g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;
            break;
        case '2':
            g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
            break;
        default:
            g_mode_type = OT_EIS_VI_ONLINE_VPROC_ONLINE;
            break;
    }
}

static ot_void sample_vio_get_vi_vproc_mode(ot_bool is_wdr_mode)
{
    ot_char ch = '0';
    ot_char end_ch;
    ot_char input[3] = {0}; /* max_len: 3 */
    ot_s32 max_len = 3; /* max_len: 3 */

    end_ch = '2';

    sample_vio_print_vi_mode_list();

    while (g_sig_flag == 0) {
        if (gets_s(input, max_len) != OT_NULL && strlen(input) == 1 && input[0] >= ch && input[0] <= end_ch) {
            break;
        } else {
            printf("\nInvalid param, please enter again!\n\n");
            sample_vio_print_vi_mode_list();
        }
        (ot_void)fflush(stdin);
    }

    sample_vio_get_vi_vproc_mode_by_char(input[0], is_wdr_mode);
}

static ot_s32 sample_vi_all_mode(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg[1] = { 0 };
    sample_vproc_attr vproc_attr[1] = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    sample_vio_get_vi_vproc_mode(OT_FALSE);

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg[0]);

    ret = sample_media_vproc_get_default_attr_by_snsor(sns_type, &vproc_attr[0]);
    if (ret != 0) {
        return ret;
    }

    if (sample_comm_vi_start_route(vi_cfg, vproc_attr, 1, g_mode_type, 1) != OT_SUCCESS) {
        return ret;
    }

    sample_get_char();

    sample_comm_vi_stop_route(vi_cfg, vproc_attr, 1, 1);
    return OT_SUCCESS;
}

static ot_s32 sample_vio_wdr(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg[1];
    sample_vproc_attr vproc_attr[1] = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    sample_vio_get_vi_vproc_mode(OT_TRUE);

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg[0]);
    ret = sample_media_vproc_get_default_attr_by_snsor(sns_type, &vproc_attr[0]);
    if(ret != 0){
        return ret;
    }

    if (sample_comm_vi_start_route(vi_cfg, vproc_attr, 1, g_mode_type, 1) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    sample_get_char();

    sample_comm_vi_stop_route(vi_cfg, vproc_attr, 1, 1);
    return OT_SUCCESS;
}

static ot_s32 sample_vi_chn_uesr_mode(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg[1];
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);
    ot_eis_handle pool_hdl = OT_NULL;
    ot_eis_img_size img_size = {0};
    sample_vo_attr *vo_attr = OT_NULL;

    sample_comm_vi_get_size_by_sns_type(sns_type, &img_size);
    vo_attr = sample_comm_args_get_vo_attr();
    if (vo_attr == OT_NULL) {
        sample_print("Invalid vo attr.\n");
        return OT_FAILURE;
    }

    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg[0]);

    if (sample_vi_sys_init(&img_size, g_mode_type)!= OT_SUCCESS) {
        return OT_FAILURE;
    }

    ret = sample_vi_user_mode_buf_pool_creat(sns_type, &pool_hdl);
    if (ret != OT_SUCCESS) {
        goto buf_pool_creat_failed;
    }

    vi_cfg[0].pipe_info[0].chn_info.chn_attr.work_mode = OT_EIS_VI_CHN_WORK_MODE_USER;
    vi_cfg[0].pipe_info[0].chn_info.chn_attr.frame_queue_depth = 3;
    vi_cfg[0].pipe_info[0].chn_info.chn_attr.pool_handle = pool_hdl;

    ret = sample_comm_vi_start_vi(&vi_cfg[0]);
    if (ret != OT_SUCCESS) {
        goto start_vi_failed;
    }

    ret = sample_comm_start_vo(vo_attr);
    if (ret != OT_SUCCESS) {
        goto start_vo_failed;
    }

    ret = sample_vi_get_frame_send_to_vo(&vi_cfg[0], vo_attr);

    sample_comm_stop_vo(vo_attr);
start_vo_failed:
    sample_comm_vi_stop_vi(&vi_cfg[0]);
start_vi_failed:
    sample_vi_user_mode_buf_pool_destroy(pool_hdl);
buf_pool_creat_failed:
    sample_comm_sys_exit();

    return ret;
}

static ot_s32 sample_vi_low_delay(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg[1] = { 0 };
    sample_vproc_attr vproc_attr[1] = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg[0]);
    ret = sample_media_vproc_get_default_attr_by_snsor(sns_type, &vproc_attr[0]);
    if(ret != 0){
        return ret;
    }

    if (sample_comm_vi_start_route(vi_cfg, vproc_attr, 1, g_mode_type, 1) != OT_SUCCESS) {
        return ret;
    }

    sample_vi_enable_low_delay(vi_cfg[0].pipe_info[0].pipe_hdl, vi_cfg[0].pipe_info[0].chn_info.chn_hdl, vproc_attr[0].chn_hdl[0]);

    sample_get_char();

    sample_comm_vi_stop_route(vi_cfg, vproc_attr, 1, 1);
    return OT_SUCCESS;
}

static ot_s32 sample_vi_two_sensor(ot_u32 display_num)
{
    ot_s32 ret;
    ot_u8 route_num = 2;
    sample_sns_type sns_type[2];
    sns_type[0] = sample_comm_args_get_sns_type(0);
    sns_type[1] = sample_comm_args_get_sns_type(1);
    ot_s32 dev_id[2] = {0, 2};
    sample_vi_cfg vi_cfg[2] = { 0 };
    sample_vproc_attr vproc_attr[2] = { 0 };

    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    for (int i = 0; i < route_num; i++ ) {
        sample_comm_vi_get_default_vi_cfg_by_dev_id(sns_type[i], &vi_cfg[i], dev_id[i]);
        sample_media_vproc_get_default_attr_by_snsor(sns_type[i], &vproc_attr[i]);
    }

    for (int i = 0; i < route_num; i++ ) {
        for (int j = 0; j < 2; j++) {
            vi_cfg[i].pipe_info[0].pipe_id[j] = 2 * i + j;
        }
    }

    if (sample_comm_vi_start_route(vi_cfg, vproc_attr, route_num, g_mode_type, display_num) != OT_SUCCESS) {
        return ret;
    }

    for (int i = 0; i < route_num; i++) {
        sample_vi_enable_low_delay(vi_cfg[i].pipe_info[0].pipe_hdl, vi_cfg[i].pipe_info[0].chn_info.chn_hdl, vproc_attr[i].chn_hdl[0]);
    }

    sample_get_char();

    sample_comm_vi_stop_route(vi_cfg, vproc_attr, 2, display_num);

    return OT_SUCCESS;
}

static ot_s32 sample_vi_one_mipi_two_sensor(ot_u32 display_num)
{
    ot_s32 ret;
    ot_u8 route_num = 2;
    ot_s32 dev_id = 0;
    sample_sns_type sns_type[2];
    sns_type[0] = sample_comm_args_get_sns_type(0);
    sns_type[1] = sample_comm_args_get_sns_type(1);
    sample_vi_cfg vi_cfg = { 0 };
    sample_vproc_attr vproc_attr[2] = { 0 };
    ot_chip_type_t chip_type = OT_CHIP_TYPE_PLUSV100;

    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    sample_comm_vi_get_default_vi_cfg_by_dev_id_and_sns_num(sns_type[0], &vi_cfg, dev_id, route_num);

    ot_bdi_get_chip_type(&chip_type);
    for (int i = 0; i < route_num; i++) {
        for (int j = 0; j < vi_cfg.pipe_num; j++) {
            vi_cfg.pipe_info[i].pipe_id[j] = 2 * i + j;
        }
        if (chip_type == OT_CHIP_TYPE_TINYV100) {
            vi_cfg.pipe_info[i].pipe_attr.image_mode.fps = 30.0;
        }
    }

    for (int i = 0; i < route_num; i++ ) {
        sample_media_vproc_get_default_attr_by_snsor(sns_type[0], &vproc_attr[i]);
    }

    if (sample_comm_vi_start_route(&vi_cfg, vproc_attr, route_num, g_mode_type, display_num) != OT_SUCCESS) {
        return ret;
    }

    for (int i = 0; i < route_num; i++) {
        sample_vi_enable_low_delay(vi_cfg.pipe_info[i].pipe_hdl, vi_cfg.pipe_info[i].chn_info.chn_hdl, vproc_attr[i].chn_hdl[0]);
    }

    sample_get_char();

    sample_comm_vi_stop_route(&vi_cfg, vproc_attr, 2, display_num);

    return OT_SUCCESS;
}


static ot_s32 sample_vi_check_params(sample_param_comb* param_comb)
{
    if ((param_comb->sample_index >= 0) && (param_comb->sample_index <= 3)) {
        // single sensor case
        if ((param_comb->input_param > ONE_MIPI_ONE_SNS_MAX) ||
            (param_comb->output_param >= SAMPLE_OUTPUT_PARAM_ONE_DISPLAY_MAX)) {
            sample_print("one sensor one display check failed.\n");
            return OT_FAILURE;
        }
    } else if (param_comb->sample_index >= 4) {
        // two sensor case
        // two sensor one display
        if (((param_comb->sample_index == 4) || (param_comb->sample_index == 6)) &&
            (param_comb->output_param != TI941_TI948_TM070JDHP08_1280X768_VO_2_PORT)) {
            sample_print("two sensor one display check failed.\n");
            return OT_FAILURE;
        }
        // two sensor two display
        if (((param_comb->sample_index == 5) || (param_comb->sample_index == 7)) &&
            ((param_comb->output_param <= SAMPLE_OUTPUT_PARAM_ONE_DISPLAY_MAX) ||
            (param_comb->output_param >= SAMPLE_OUTPUT_PARAM_MAX))) {
                sample_print("two sensor two display check failed.\n");
                return OT_FAILURE;
        }
    }
    return OT_SUCCESS;
}

static ot_void sample_vio_usage(const char *prg_name)
{
    printf("usage : %s <index> -i <input_param> -o <output_param>\n", prg_name);
    printf("index:\n");
    printf("    (0) all mode route          :vi linear(Online/Offline) -> vproc(Online/Offline) -> vo.\n");
    printf("    (1) wdr route               :vi wdr(Online) -> vproc(Offline) -> vo.\n");
    printf("    (2) chn user mode           :vi linear(Offline) -> vo.\n");
    printf("    (3) low delay               :vi(pipe & chn low delay) -> vproc(chn low delay) -> vo.\n");
    printf("    (4) two sensor(low delay)   :vi -> vproc -> vo.\n");
    printf("    (5) two sensor(low delay) with two display panel  :vi-> vproc-> vo.\n");
    printf("    (6) one mipi two sensor(low delay) :vi-> vproc-> vo.\n");
    printf("    (7) one mipi two sensor(low delay) with two display panel  :vi-> vproc-> vo.\n");
    sample_comm_args_help();
    printf("\neg: %s 4 -i 20 -o 4\n\n", prg_name);
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

    switch (case_index) {
        case 0: /* 0 all mode route */
            ret = sample_vi_all_mode();
            break;
        case 1: /* 1 wdr route */
            ret = sample_vio_wdr();
            break;
        case 2:
            ret = sample_vi_chn_uesr_mode();
            break;
        case 3:
            ret = sample_vi_low_delay();
            break;
        case 4:
            ret = sample_vi_two_sensor(1);
            break;
        case 5:
            ret = sample_vi_two_sensor(2);
            break;
        case 6:
            ret = sample_vi_one_mipi_two_sensor(1);
            break;
        case 7:
            ret = sample_vi_one_mipi_two_sensor(2);
            break;
        default:
            ret = OT_FAILURE;
            break;
    }

    return ret;
}

ot_s32 main(ot_s32 argc, ot_char *argv[])
{
    ot_s32 ret;
    ot_u32 index;
    sample_param_comb *param_comb = OT_NULL;

    if ((argc < 2) || (argc > 6)) { /* 2:arg num */
        sample_vio_usage(argv[0]);
        return OT_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_vio_usage(argv[0]);
        return OT_FAILURE;
    }

    if (strlen(argv[1]) > 2 || strlen(argv[1]) <= 0 || !check_digit(argv[1][0]) || /* 2:arg len */
        (strlen(argv[1]) == 2 && (!check_digit(argv[1][1]) || argv[1][0] == '0'))) { /* 2:arg len */
        sample_vio_usage(argv[0]);
        return OT_FAILURE;
    }

    if (strlen(argv[1]) == 2 && argv[1][1] != '0') { /* 2:arg len, max: 10 */
        sample_vio_usage(argv[0]);
        return OT_FAILURE;
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
    ret = sample_vi_check_params(param_comb);
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
