/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include "sample_comm.h"

#define SAMPLE_STREAM_PATH "./source_file"
#define REF_NUM 2
#define DISPLAY_NUM 2
#define MAX_FRM_WIDTH 8192
#define OT_H264D_MAX_HEIGHT                6144
#define OT_H264D_MAX_WIDTH                 6144
#define OT_H265D_MAX_WIDTH                 6144
#define OT_H265D_MAX_HEIGHT                6144
#define OT_JPEGD_MAX_WIDTH                 16384
#define OT_JPEGD_MAX_HEIGHT                16384
#define OT_H264D_ALIGN_W                   64
#define OT_H264D_ALIGN_H                   16
#define OT_H265D_ALIGN_W                   64
#define OT_H265D_ALIGN_H                   64
#define OT_JPEGD_ALIGN_W                   64
#define OT_JPEGD_ALIGN_H                   16

static ot_s32 g_sample_exit = 0;
static FILE *g_vdec_pfd = OT_NULL;
char *g_v_buf_virt_y = OT_NULL;
char *g_v_buf_virt_c = OT_NULL;
ot_u32 g_ysize, g_csize;
char g_vdec_file[] = "3840x2160_8bit.h265";

static ot_bool g_send_vo_pthread = OT_FALSE;
extern sample_vo_attr *g_vo_attr;
static volatile sig_atomic_t g_sig_flag = 0;

typedef struct {
    ot_eis_handle vdec_chn_hdl;
    ot_eis_handle vo_inport_hdl;
} vdec_chn_send_attr;


// default vdec attr for reference
static sample_vdec_attr g_sample_vdec_attr = {
    .chn_attr = {
        .format = OT_VDEC_CODEC_HEVC,
        .width = OT_H265D_MAX_WIDTH,
        .height = OT_H265D_MAX_HEIGHT,
        .stream_buf_size = (OT_H265D_MAX_WIDTH*OT_H265D_MAX_HEIGHT),
        .frame_buf_size = 56844288,
        .frame_buf_cnt = 5,
        .ref_frame_num = 2,
        .output_order = OT_VDEC_ORDER_DISPLAY,
    },
    .vdec_hdl = (ot_eis_vdec_chn_handle)-1,
};

static ot_void sample_vdec_handle_sig(ot_s32 signo)
{
    if ((signo == SIGINT) || (signo == SIGTERM)) {
        g_sample_exit = 1;
    }
}

static ot_void sample_vdec_usage(const char *s_prg_nm)
{
    printf("\n/************************************/\n");
    printf("usage : %s <index> -o <screen_type>\n", s_prg_nm);
    printf("index:\n");
    printf("\t0:  VDEC(H265) TO VO\n");
    printf("\t1:  VDEC(H264) TO VO\n");
    printf("\t2:  VDEC(JPEG) TO VO\n");
    sample_comm_vo_args_help();
    printf("default usage (BOE screen) : %s <index> -o 2\n", s_prg_nm);
    // printf("\t3:  VDEC(JPEG) TO FILE\n");
    printf("/************************************/\n\n");
}

static ot_s32 sample_vdec_check_params(sample_param_comb* param_comb)
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

static ot_bool ot_media_vdec_check_pic_size(ot_vdec_codec_format format, ot_u32 width, ot_u32 height)
{
    if ((format == OT_VDEC_CODEC_HEVC) && (width <= OT_H265D_MAX_WIDTH) && (height <= OT_H265D_MAX_HEIGHT)) {
        return OT_TRUE;
    }
    if ((format == OT_VDEC_CODEC_H264) && (width <= OT_H264D_MAX_WIDTH) && (height <= OT_H264D_MAX_HEIGHT)) {
        return OT_TRUE;
    }
    if ((format == OT_VDEC_CODEC_JPEG) && (width <= OT_JPEGD_MAX_WIDTH) && (height <= OT_JPEGD_MAX_HEIGHT)) {
        return OT_TRUE;
    }
    return OT_FALSE;
}

static ot_u32 ot_media_vdec_get_pic_buf_size(ot_vdec_codec_format format, const ot_eis_img_attr *img_attr)
{
    ot_u32 align_width, align_height;
    ot_u32 header_size;
    ot_u32 header_stride;
    ot_u32 extra_size = 0;
    ot_u32 size;

    if (img_attr == OT_NULL) {
        return 0;
    }

    if (ot_media_vdec_check_pic_size(format, img_attr->width, img_attr->height) == OT_FALSE) {
        return 0;
    }

    if (format == OT_VDEC_CODEC_H264) {
        align_width = ot_eis_up_align(img_attr->width, OT_H264D_ALIGN_W);
        align_height = ot_eis_up_align(img_attr->height, OT_H264D_ALIGN_H);
        header_stride = ot_eis_up_align(align_width, 2048) >> 6;
        header_size = header_stride * (ot_eis_up_align(align_height, OT_H264D_ALIGN_H) >> 2);
        size = ((header_size + align_width * align_height) * 3) >> 1;
    } else if (format == OT_VDEC_CODEC_HEVC) {
        align_width = ot_eis_up_align(img_attr->width, OT_H265D_ALIGN_W);
        align_height = ot_eis_up_align(img_attr->height, OT_H265D_ALIGN_H);
        header_stride = ot_eis_up_align(align_width, 2048) >> 6;
        header_size = header_stride * (ot_eis_up_align(align_height, OT_H265D_ALIGN_H) >> 2);
        size = ((header_size + align_width * align_height + extra_size) * 3) >> 1;
    } else if (format == OT_VDEC_CODEC_JPEG) {
        // OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR
        align_width = ot_eis_up_align(img_attr->width, OT_JPEGD_ALIGN_W);
        align_height = ot_eis_up_align(img_attr->height, OT_JPEGD_ALIGN_H);
        if (img_attr->pixel_fmt == OT_EIS_IMAGE_FORMAT_YVU_422_SEMIPLANAR) {
            size = align_width * align_height * 2; /* 2:Y+1/2U+1/2V */
        } else {
            size = (align_width * align_height * 3) >> 1; /* 3:Y+1/4U+1/4V */
        }
    } else {
        size = 0;
    }

    return size;
}

ot_void sample_vdec_init_vdec_attr(sample_vdec_attr *vdec_attr)
{
    ot_eis_img_attr img_attr = { 0 };
    img_attr.height = vdec_attr->chn_attr.height;
    img_attr.width = vdec_attr->chn_attr.width;
    img_attr.bit_depth = OT_EIS_PIXEL_BIT_DEPTH_8;
    img_attr.pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR;

    vdec_attr->chn_attr.frame_buf_size = ot_media_vdec_get_pic_buf_size(vdec_attr->chn_attr.format, &img_attr);
    vdec_attr->chn_attr.stream_buf_size = vdec_attr->chn_attr.width * vdec_attr->chn_attr.height;
    if (vdec_attr->chn_attr.format == OT_VDEC_CODEC_JPEG) {
        vdec_attr->chn_attr.frame_buf_cnt = DISPLAY_NUM + 1;
        vdec_attr->chn_attr.ref_frame_num = 0;
    } else {
        vdec_attr->chn_attr.frame_buf_cnt = REF_NUM + DISPLAY_NUM + 1;
        vdec_attr->chn_attr.ref_frame_num = REF_NUM;
    }
    vdec_attr->chn_attr.output_order = OT_VDEC_ORDER_DISPLAY;
}

static ot_s32 sample_start_vdec(sample_vdec_attr *vdec_chn_attr)
{
    ot_s32 ret;

    ret = sample_comm_vdec_start(vdec_chn_attr);
    if (ret != OT_SUCCESS) {
        sample_comm_vdec_stop(vdec_chn_attr->vdec_hdl);
        return OT_FAILURE;
    }

    return ret;
}

static ot_void sample_read_file_to_vdec(sample_vdec_attr *vdec_attr, const char *stream_name, vdec_thread_param *vdec_send, pthread_t *vdec_thread)
{
    if (snprintf_s(vdec_send->c_file_name, sizeof(vdec_send->c_file_name), sizeof(vdec_send->c_file_name) - 1,
        stream_name) < 0) {
        return;
    }
    if (snprintf_s(vdec_send->c_file_path, sizeof(vdec_send->c_file_path), sizeof(vdec_send->c_file_path) - 1,
        "%s", SAMPLE_STREAM_PATH) < 0) {
        return;
    }
    vdec_send->type          = vdec_attr->chn_attr.format;
    vdec_send->chn_hdl       = vdec_attr->vdec_hdl;
    vdec_send->interval_time = 1000; /* 1000: interval time */
    vdec_send->pts_init      = 0;
    vdec_send->pts_increase  = 0;
    vdec_send->e_thread_ctrl = THREAD_CTRL_START;
    vdec_send->circle_send   = OT_TRUE;
    vdec_send->milli_sec     = 0;
    vdec_send->min_buf_size  = (vdec_attr->chn_attr.width * vdec_attr->chn_attr.height * 3) >> 1; /* 3:yuv */
    vdec_send->fps           = 30; /* 30:frame rate */

    sample_comm_vdec_start_send_stream(vdec_send, vdec_thread);
}

static ot_s32 sample_vio_sys_init(ot_eis_img_size *img_size)
{
    ot_video_buffer_attr vrb_cfg;

    sample_common_get_vrb_cfg(3, img_size->width, img_size->height, &vrb_cfg);
    if (sample_comm_sys_init(&vrb_cfg) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

static ot_void *sample_vdec_get_frame_send_to_vo_thread(ot_void *param)
{
    ot_s32 ret;
    ot_eis_handle vdec_chn_hdl;
    ot_eis_handle vo_inport_hdl;
    vdec_chn_send_attr *send_attr = (vdec_chn_send_attr *)param;
    ot_eis_vdec_pic_frame vdec_frame = { 0 };
    ot_s32 milli_sec = -1;

    vdec_chn_hdl = send_attr->vdec_chn_hdl;
    vo_inport_hdl = send_attr->vo_inport_hdl;
    while (g_send_vo_pthread) {
        ret = ot_eis_vdec_chn_acquire_frame(vdec_chn_hdl, &vdec_frame, milli_sec);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vdec_chn_acquire_frame failed with %#x\n", ret);
            continue;
        }

        ret = ot_eis_vo_in_port_send_frame(vo_inport_hdl, &vdec_frame.image);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vo_in_port_send_frame failed with %#x\n", ret);
            ot_eis_vdec_chn_release_frame(vdec_chn_hdl, &vdec_frame);
            continue;
        }

        ret = ot_eis_vdec_chn_release_frame(vdec_chn_hdl, &vdec_frame);
        if (ret != OT_SUCCESS) {
            continue;
        }
    }

    return OT_NULL;
}

static ot_s32 sample_vdec_get_frame_send_to_vo(sample_vdec_attr *vdec_cfg, sample_vo_attr *vo_attr)
{
    ot_s32 ret = OT_SUCCESS;
    pthread_t send_vdec_thread = 0;
    pthread_t send_vo_thread = 0;
    vdec_chn_send_attr send_attr = {0};
    send_attr.vdec_chn_hdl = vdec_cfg->vdec_hdl;
    send_attr.vo_inport_hdl = vo_attr->in_port_handle[0];

    vdec_thread_param vdec_send = {0};

    // read file and send frame to vdec
    sample_read_file_to_vdec(&g_sample_vdec_attr, g_vdec_file, &vdec_send, &send_vdec_thread);

    // get frame from vdec and send to vo for display
    g_send_vo_pthread = OT_TRUE;
    ret = pthread_create(&send_vo_thread, OT_NULL, sample_vdec_get_frame_send_to_vo_thread, (ot_void *)&send_attr);
    if (ret != OT_SUCCESS) {
        printf("vdec create send vo frame thread failed!\n");
        g_send_vo_pthread = OT_FALSE;
        return OT_FAILURE;
    }

    printf("frames thread running, print any key to exit!\n");
    getchar();

    g_send_vo_pthread = OT_FALSE;
    pthread_join(send_vo_thread, OT_NULL);
    vdec_send.e_thread_ctrl = THREAD_CTRL_STOP;
    pthread_join(send_vdec_thread, OT_NULL);

    return ret;
}

static ot_s32 sample_h265_vdec_to_vo()
{
    ot_s32 ret;
    ot_eis_img_size img_size = {0};
    sample_vo_attr *vo_attr = OT_NULL;

    sample_comm_vi_get_size_by_sns_type(SENSOR0_TYPE, &img_size);
    if (sample_vio_sys_init(&img_size) != OT_SUCCESS) {
        return OT_FAILURE;
    }
    vo_attr = g_vo_attr;
    if (vo_attr == OT_NULL) {
        sample_print("Invalid vo attr.\n");
        return OT_FAILURE;
    }
    ret = sample_comm_start_vo(vo_attr);
    if (ret != OT_SUCCESS) {
        goto start_vo_failed;
    }

    sample_vdec_init_vdec_attr(&g_sample_vdec_attr);
    ret = sample_start_vdec(&g_sample_vdec_attr);
    if (ret != OT_SUCCESS) {
        goto start_vdec_failed;
    }

    ret = sample_vdec_get_frame_send_to_vo(&g_sample_vdec_attr, vo_attr);
    if (ret != OT_SUCCESS) {
        sample_print("vdec send to vo failed! ret : %#x\n", ret);
    }

    sample_comm_vdec_stop(g_sample_vdec_attr.vdec_hdl);

start_vdec_failed:
    sample_comm_stop_vo(vo_attr);
start_vo_failed:
    sample_comm_sys_exit();

    return ret;
}

static ot_s32 sample_h264_vdec_to_vo()
{
    g_sample_vdec_attr.chn_attr.format = OT_VDEC_CODEC_H264;
    g_sample_vdec_attr.chn_attr.width = OT_H264D_MAX_WIDTH;
    g_sample_vdec_attr.chn_attr.height = OT_H264D_MAX_HEIGHT;
    strcpy(g_vdec_file, "3840x2160_8bit.h264");
    return sample_h265_vdec_to_vo();
}

static ot_s32 sample_jpeg_vdec_to_vo()
{
    g_sample_vdec_attr.chn_attr.format = OT_VDEC_CODEC_JPEG;
    g_sample_vdec_attr.chn_attr.width = 3840;
    g_sample_vdec_attr.chn_attr.height = 2160;
    strcpy(g_vdec_file, "3840x2160.jpg");
    return sample_h265_vdec_to_vo();
}

static ot_s32 sample_vdec_execute_case(int index)
{
    ot_s32 ret;

    switch (index) {
        case 0: {
            ret = sample_h265_vdec_to_vo();
            break;
        }
        case 1: {
            ret = sample_h264_vdec_to_vo();
            break;
        }
        case 2: {
            ret = sample_jpeg_vdec_to_vo();
            break;
        }
        default: {
            ret = OT_FAILURE;
            break;
        }
    }

    return ret;
}

/******************************************************************************
* function    : main()
* description : video vdec sample
******************************************************************************/
int main(int argc, char *argv[])
{
    ot_s32 ret;
    ot_u32 index;
    sample_param_comb *param_comb = OT_NULL;

    if ((argc < 2) || (argc > 6)) { /* 2:arg num */
        sample_vdec_usage(argv[0]);
        return OT_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_vdec_usage(argv[0]);
        return OT_FAILURE;
    }

    if (strlen(argv[1]) > 2 || strlen(argv[1]) <= 0 || !check_digit(argv[1][0]) || /* 2:arg len */
        (strlen(argv[1]) == 2 && (!check_digit(argv[1][1]) || argv[1][0] == '0'))) { /* 2:arg len */
        sample_vdec_usage(argv[0]);
        return OT_FAILURE;
    }

    if (strlen(argv[1]) == 2 && argv[1][1] != '0') { /* 2:arg len, max: 10 */
        sample_vdec_usage(argv[0]);
        return OT_FAILURE;
    }

    sample_vdec_handle_sig(sample_vdec_handle_sig);

    index = strtol(argv[1], NULL, 10); /* base 10 */
    sample_comm_args_set_sample_index(index);

    ret = sample_comm_args_register(argc, argv);
    if (ret != OT_SUCCESS) {
        sample_vdec_usage(argv[0]);
        return OT_FAILURE;
    }

    param_comb = sample_comm_args_get_param_comb();
    ret = sample_vdec_check_params(param_comb);
    if (ret != OT_SUCCESS) {
        sample_vdec_usage(argv[0]);
        return OT_FAILURE;
    }

    ret = sample_vdec_execute_case(index);
    if ((ret == OT_SUCCESS) && (g_sig_flag == 0)) {
        printf("\033[0;32mprogram exit normally!\033[0;39m\n");
    } else {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    return ret;
}
