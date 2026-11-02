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
static ot_u32 g_route_num = 1;
static ot_u32 g_display_num = 1;
static sample_vi_cfg g_vi_cfg[SAMPLE_VIO_MAX_ROUTE_NUM] = { 0 };
static sample_vproc_attr g_vproc_attr[SAMPLE_VIO_MAX_ROUTE_NUM] = { 0 };

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

static ot_s32 sample_quickboot_all_mode(ot_void)
{
    ot_s32 ret;
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);
    ot_eis_vi_vproc_mode_type mode_type;

    sample_comm_vi_get_default_vi_cfg(sns_type, &g_vi_cfg[0]);
    ret = sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_vproc_attr[0]);
    if (ret != 0) {
        return ret;
    }
    g_vi_cfg[0].pipe_info[0].quickboot_en = OT_TRUE;

    mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    if (sample_comm_vi_start_route(g_vi_cfg, g_vproc_attr,
        g_route_num, mode_type, g_display_num) != OT_SUCCESS) {
        return ret;
    }

    sample_vi_enable_low_delay(g_vi_cfg[0].pipe_info[0].pipe_hdl,
        g_vi_cfg[0].pipe_info[0].chn_info.chn_hdl, g_vproc_attr[0].chn_hdl[0]);

    while (1) {
        if (g_sig_flag == 1) {
            break;
        }
    }

    return OT_SUCCESS;
}

static ot_s32 sample_quickboot_2mipi_2sensor(ot_void)
{
    ot_s32 ret;
    sample_sns_type sns_type[2];
    sns_type[0] = sample_comm_args_get_sns_type(0);
    sns_type[1] = sample_comm_args_get_sns_type(1);
    ot_s32 dev_id[2] = {0, 2};
    ot_eis_vi_vproc_mode_type mode_type;

    for (int i = 0; i < g_route_num; i++ ) {
        sample_comm_vi_get_default_vi_cfg_by_dev_id(sns_type[i], &g_vi_cfg[i], dev_id[i]);
        g_vi_cfg[i].pipe_info[0].pipe_id[0] = 2 * i;
        g_vi_cfg[i].pipe_info[0].pipe_id[1] = 2 * i + 1;
        g_vi_cfg[i].pipe_info[0].quickboot_en = OT_TRUE;
        sample_media_vproc_get_default_attr_by_snsor(sns_type[i], &g_vproc_attr[i]);
    }

    mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    if (sample_comm_vi_start_route(g_vi_cfg, g_vproc_attr,
        g_route_num, mode_type, g_display_num) != OT_SUCCESS) {
        return ret;
    }

    for (int i = 0; i < g_route_num; i++) {
        sample_vi_enable_low_delay(g_vi_cfg[i].pipe_info[0].pipe_hdl,
            g_vi_cfg[i].pipe_info[0].chn_info.chn_hdl, g_vproc_attr[i].chn_hdl[0]);
    }

    while (1) {
        if (g_sig_flag == 1) {
            break;
        }
    }

    return OT_SUCCESS;
}

static ot_s32 sample_quickboot_1mipi_2sensor(ot_void)
{
    ot_s32 ret;
    ot_u8 route_num = 2;
    ot_s32 dev_id = 0;
    sample_sns_type sns_type[2];
    ot_eis_vi_vproc_mode_type mode_type;

    sns_type[0] = sample_comm_args_get_sns_type(0);
    sns_type[1] = sample_comm_args_get_sns_type(1);

    sample_comm_vi_get_default_vi_cfg_by_dev_id_and_sns_num(sns_type[0], &g_vi_cfg[0], dev_id, route_num);
    g_vi_cfg[0].pipe_info[0].quickboot_en = OT_TRUE;
    g_vi_cfg[0].pipe_info[1].quickboot_en = OT_TRUE;
    g_vi_cfg[0].pipe_info[0].pipe_id[0] = 0;
    g_vi_cfg[0].pipe_info[1].pipe_id[0] = 1;

    for (int i = 0; i < route_num; i++ ) {
        sample_media_vproc_get_default_attr_by_snsor(sns_type[0], &g_vproc_attr[i]);
    }

    mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    if (sample_comm_vi_start_route(g_vi_cfg, g_vproc_attr,
        g_route_num, mode_type, g_display_num) != OT_SUCCESS) {
        return ret;
    }

    for (int i = 0; i < route_num; i++) {
        sample_vi_enable_low_delay(g_vi_cfg[0].pipe_info[i].pipe_hdl,
            g_vi_cfg[0].pipe_info[i].chn_info.chn_hdl, g_vproc_attr[i].chn_hdl[0]);
    }

    while (1) {
        if (g_sig_flag == 1) {
            break;
        }
    }

    return OT_SUCCESS;
}

static ot_void sample_quickboot_handle_sig(ot_s32 signo)
{
    if ((signo == SIGINT) || (signo == SIGTERM)) {
        printf("\033[0;31m\nprogram exit!\033[0;39m\n");
        sample_comm_vi_stop_route(g_vi_cfg, g_vproc_attr, g_route_num, g_display_num);
        g_sig_flag = 1;
    }
}

static ot_void sample_quickboot_usage(const char *prg_name)
{
    printf("usage : %s <index> -i <input_param> -o <output_param>\n", prg_name);
    printf("index:\n");
    printf("    (0) single sensor          :vi (Offline) -> vproc(Offline) -> vo.\n");
    printf("    (1) two mipi two sensor    :vi (Offline) -> vproc(Offline) -> vo.\n");
    printf("    (2) one mipi two sensor    :vi (Offline) -> vproc(Offline) -> vo.\n");
    sample_comm_args_help();
    printf("\neg: %s 0 -i 0 -o 0\n\n", prg_name);
}

static ot_s32 sample_quickboot_check_params(sample_param_comb* param_comb)
{
    if ((param_comb->sample_index == 0) && (param_comb->input_param < ONE_MIPI_ONE_SNS_MAX)) {
        // one sensor one display
        if (param_comb->output_param >= TI941_TI948_TM070JDHP08_1280X768_VO_2_PORT) {
            sample_print("one sensor one display check failed.\n");
            return OT_FAILURE;
        }
    } else if ((param_comb->sample_index <= 2) && (param_comb->input_param > ONE_MIPI_ONE_SNS_MAX)) {
        // two sensor two display
        if ((param_comb->output_param <= SAMPLE_OUTPUT_PARAM_ONE_DISPLAY_MAX) ||
            (param_comb->output_param >= SAMPLE_OUTPUT_PARAM_MAX)) {
            sample_print("two sensor two display check failed.\n");
            return OT_FAILURE;
        }
    } else {
        sample_print("Invalid params\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
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

static ot_s32 sample_quickboot_execute_case(ot_u32 case_index)
{
    ot_s32 ret;

    switch (case_index) {
        case 0:
            g_route_num = 1;
            g_display_num = 1;
            ret = sample_quickboot_all_mode();
            break;
        case 1:
            g_route_num = 2;
            g_display_num = 2;
            ret = sample_quickboot_2mipi_2sensor();
            break;
        case 2:
            g_route_num = 2;
            g_display_num = 2;
            ret = sample_quickboot_1mipi_2sensor();
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
        sample_quickboot_usage(argv[0]);
        return OT_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_quickboot_usage(argv[0]);
        return OT_FAILURE;
    }

    if (strlen(argv[1]) > 2 || strlen(argv[1]) <= 0 || !check_digit(argv[1][0]) || /* 2:arg len */
        (strlen(argv[1]) == 2 && (!check_digit(argv[1][1]) || argv[1][0] == '0'))) { /* 2:arg len */
        sample_quickboot_usage(argv[0]);
        return OT_FAILURE;
    }

    if (strlen(argv[1]) == 2 && argv[1][1] != '0') { /* 2:arg len, max: 10 */
        sample_quickboot_usage(argv[0]);
        return OT_FAILURE;
    }

    sample_register_sig_handler(sample_quickboot_handle_sig);

    index = strtol(argv[1], NULL, 10); /* base 10 */
    sample_comm_args_set_sample_index(index);

    ret = sample_comm_args_register(argc, argv);
    if (ret != OT_SUCCESS) {
        sample_print("register params failed.\n");
        sample_quickboot_usage(argv[0]);
        return OT_FAILURE;
    }

    param_comb = sample_comm_args_get_param_comb();
    ret = sample_quickboot_check_params(param_comb);
    if (ret != OT_SUCCESS) {
        sample_print("check params failed.\n");
        sample_quickboot_usage(argv[0]);
        return OT_FAILURE;
    }

    ret = sample_quickboot_execute_case(index);
    if ((ret == OT_SUCCESS) && (g_sig_flag == 0)) {
        printf("\033[0;32mprogram exit normally!\033[0;39m\n");
    } else {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    return ret;
}
