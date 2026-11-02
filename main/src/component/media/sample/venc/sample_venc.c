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
#include <dlfcn.h>
#include <sys/prctl.h>

#include "sample_comm.h"
#include "securec.h"
#include "ot_buffer_pool.h"
#include "ot_eis_vi_api.h"

#define SAMPLE_RETURN_JPEG  1
static ot_u64 g_jpeg_acquire_cnt = 0;

static volatile sig_atomic_t g_sig_flag = 0;
static FILE* g_file = OT_NULL;
static ot_bool g_send_chn_pthread = OT_FALSE;
static ot_bool g_save_file_pthread = OT_FALSE;
extern sample_vi_attr *g_vi_attr;

typedef struct {
    ot_eis_handle vi_chn_hdl;
    ot_eis_handle vo_inport_hdl;
} vi_chn_send_attr;

typedef struct {
    ot_eis_handle vi_chn_hdl;
    ot_eis_handle venc_hdl;
} vi_send_attr;

static sample_venc_attr g_sample_venc_attr = {
    .venc_attr = {
        .codecFormat = OT_EIS_VENC_CODEC_HEVC,
        .common_attr = {
            .max_pic_width = 2688,//3840
            .max_pic_height = 1520,//2160
            .out_width = 2688,//3840
            .out_height = 1520,//2160
            .stream_buf_size = 6220800,
        },
        .video_attr = {
            .rc_attr = {
                .gop = 60,
                .output_fps = 30,
                .input_fps = 60,
                .rc_mode = OT_EIS_VENC_RC_MODE_CVBR,
                .cvbr_attr = {
                    .statis_time = 1,
                    .max_bit_rate = 13312,
                    .short_statis_time = 1,
                    .long_statis_time = 1,
                    .long_max_bit_rate = 13312,
                    .long_min_bit_rate = 3072,
                },
            },
            .gop_attr = {
                .gop_mode = OT_EIS_VENC_GOP_MODE_NORMAL,
                .gop_normal = {
                    .qp_delta_ip = 2,
                },
            },
        },
    },
    .venc_hdl = (ot_eis_venc_chn_handle)-1,
};

ot_eis_vi_vproc_mode_type g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;

static ot_s32 sample_vio_sys_init(ot_eis_img_size *img_size)
{
    ot_video_buffer_attr buf_cfg;

    sample_common_get_vrb_cfg(3, img_size->width, img_size->height, &buf_cfg);
    if (sample_comm_sys_init(&buf_cfg) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    if (sample_comm_sys_set_vi_vproc_init_cfg(g_mode_type) != OT_SUCCESS) {
        goto sys_exit;
    }

    return OT_SUCCESS;
sys_exit:
    sample_comm_sys_exit();
    return OT_FAILURE;
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

static ot_void* sample_vi_get_frame_send_to_venc_thread(ot_void* param)
{
    ot_s32 ret;
    ot_eis_handle vi_chn_hdl;
    ot_eis_handle venc_hdl;
    vi_send_attr* send_attr = (vi_send_attr*)param;
    ot_eis_video_frame video_frame = { 0 };
    ot_eis_venc_image_frame venc_frame = { 0 };
    ot_s32 milli_sec = -1;

    vi_chn_hdl = send_attr->vi_chn_hdl;
    venc_hdl = send_attr->venc_hdl;
    while (g_send_chn_pthread) {
        ret = ot_eis_vi_chn_get_frame(vi_chn_hdl, &video_frame, milli_sec);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vi_chn_get_frame failed with %#x\n", ret);
            goto exit;
        }

        venc_frame.frame = &video_frame;
        ret = ot_eis_venc_send_frame(venc_hdl, &venc_frame, -1);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_venc_send_frame failed with %#x\n", ret);
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

/* get file postfix according palyload_type. */
ot_s32 sample_comm_venc_get_file_postfix(ot_eis_venc_codec_format payload, ot_char* file_postfix, ot_u8 len)
{
    if (payload == OT_EIS_VENC_CODEC_H264) {
        if (strcpy_s(file_postfix, len, ".h264") != EOK) {
            return OT_FAILURE;
        }
    }
    else if (payload == OT_EIS_VENC_CODEC_HEVC) {
        if (strcpy_s(file_postfix, len, ".h265") != EOK) {
            return OT_FAILURE;
        }
    }
    else if (payload == OT_EIS_VENC_CODEC_JPEG) {
        if (strcpy_s(file_postfix, len, ".jpg") != EOK) {
            return OT_FAILURE;
        }
    }
    else {
        sample_print("payload type err!\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_comm_set_file_name(ot_eis_venc_chn_handle venc_chn, ot_char* file_postfix, ot_char* real_file_name)
{
    ot_char file_name[FILE_NAME_LEN];
    if (snprintf_s(file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1, "./") < 0) {
        return OT_FAILURE;
    }

    if (realpath(file_name, real_file_name) == OT_NULL) {
        sample_print("chn[%d] stream file path error\n", (ot_s32)((intptr_t)venc_chn));
        return OT_FAILURE;
    }

    if (snprintf_s(real_file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1,
        "stream_chn%d%s", (ot_s32)((intptr_t)venc_chn), file_postfix) < 0) {
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_comm_set_name_save_stream(sample_venc_attr* attr)
{
    ot_s32 ret, fd;
    ot_char real_file_name[PATH_MAX] = { 0 };

    ot_eis_venc_codec_format payload_type = attr->venc_attr.codecFormat;
    ot_char file_postfix[10];

    ret = sample_comm_venc_get_file_postfix(payload_type, file_postfix, sizeof(file_postfix));
    if (ret != OT_SUCCESS) {
        sample_print("sample_comm_venc_get_file_postfix [%d] failed with %#x!\n", payload_type, ret);
        return ret;
    }

    ret = sample_comm_set_file_name(attr->venc_hdl, file_postfix, real_file_name);
    if (ret != OT_SUCCESS) {
        return ret;
    }

    g_file = fopen(real_file_name, "wb");
    if (!g_file) {
        sample_print("open file[%s] failed!\n", real_file_name);
        return OT_FAILURE;
    }
    fd = fileno(g_file);
    fchmod(fd, S_IRUSR | S_IWUSR);

    return OT_SUCCESS;
}

static ot_s32 test_sample_comm_save_frame_to_file(FILE* fd, ot_eis_venc_stream* stream)
{
    // Attention, attention, attention: No time-consuming operations are allowed here, otherwise the image delay and stalling will occur.
    // Suggestion: Use ringBuffer to save the stream.  The user needs to copy the stream into the ringBuffer and then manage the ringbuffer

    return OT_SUCCESS;
}

static ot_s32 sample_comm_get_stream_from_one_channl(sample_venc_attr* attr)
{
    ot_s32 ret;
    ot_eis_venc_chn_status status = { 0 };

    ret = ot_eis_venc_chn_query_status(attr->venc_hdl, &status);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_query_status chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)attr->venc_hdl), ret);
        return ret;
    }

    if (status.unTaken_streams == 0) {
        usleep(10000);
        return ret;
    }

    ot_eis_venc_stream ot_stream = { 0 };
    ret = ot_eis_venc_chn_acquire_stream(g_sample_venc_attr.venc_hdl, &ot_stream, -1);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_acquire_stream chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl), ret);
        return ret;
    }

    g_jpeg_acquire_cnt = g_jpeg_acquire_cnt + 1;
    if (attr->venc_attr.codecFormat != OT_EIS_VENC_CODEC_JPEG ||
        (attr->venc_attr.codecFormat == OT_EIS_VENC_CODEC_JPEG && g_jpeg_acquire_cnt == 10)) {
        ret = test_sample_comm_save_frame_to_file(g_file, &ot_stream);
        if (ret != OT_SUCCESS) {
            sample_print("test_sample_comm_save_frame_to_file chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl), ret);
        }
    }

    ret = ot_eis_venc_chn_release_stream(g_sample_venc_attr.venc_hdl, &ot_stream);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_release_stream chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl), ret);
        return ret;
    }

    if (attr->venc_attr.codecFormat == OT_EIS_VENC_CODEC_JPEG  && g_jpeg_acquire_cnt == 10) {
        return SAMPLE_RETURN_JPEG;
    }

    return OT_SUCCESS;
}

/* get stream from each channels and save them */
ot_void* sample_comm_venc_get_venc_stream_proc(ot_void* p)
{
    ot_s32 ret;

    prctl(PR_SET_NAME, "get_venc_stream", 0, 0, 0);

    // ret = sample_comm_set_name_save_stream(&g_sample_venc_attr);
    // if (ret != OT_SUCCESS) {
    //     sample_print("set name save stream failed\n");
    //     return OT_NULL;
    // }

    while (g_save_file_pthread == OT_TRUE) {
        ret = sample_comm_get_stream_from_one_channl(&g_sample_venc_attr);
        if (ret == SAMPLE_RETURN_JPEG) {
            sample_print("sample_comm_get_stream_from_one_channl chn[%d] JPEG done!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl));
            break;
        }

        if (ret != OT_SUCCESS) {
            sample_print("sample_comm_get_stream_from_one_channl chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl), ret);
        }
    }

    sample_print("sample_venc_chn[%d] success get %lld frame streams!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl), g_jpeg_acquire_cnt);

    g_send_chn_pthread = OT_FALSE;
    // fclose(g_file);
    return OT_NULL;
}


static ot_s32 sample_vi_frame_to_venc_to_file(sample_vi_cfg* vi_cfg, sample_venc_attr* venc_attr)
{
    ot_s32 ret;
    pthread_t thread_id = 0;
    pthread_t save_pid = 0;
    vi_send_attr send_attr = { 0 };
    send_attr.vi_chn_hdl = vi_cfg->pipe_info[0].chn_info.chn_hdl;
    send_attr.venc_hdl = venc_attr->venc_hdl;

    g_send_chn_pthread = OT_TRUE;
    ret = pthread_create(&thread_id, OT_NULL, sample_vi_get_frame_send_to_venc_thread, (ot_void*)&send_attr);
    if (ret != OT_SUCCESS) {
        printf("create send frame thread failed!\n");
        g_send_chn_pthread = OT_FALSE;
        return OT_FAILURE;
    }

    // 从venc取帧存入文件
    g_save_file_pthread = OT_TRUE;
    ret = pthread_create(&save_pid, OT_NULL, sample_comm_venc_get_venc_stream_proc, OT_NULL);
    if (ret != OT_SUCCESS) {
        printf("create save frame thread failed!\n");
        g_save_file_pthread = OT_FALSE;
        return OT_FAILURE;
    }

    printf("threads running, print any key to exit!\n");
    getchar();

    g_save_file_pthread = OT_FALSE;
    g_send_chn_pthread = OT_FALSE;

    pthread_join(save_pid, OT_NULL);
    pthread_join(thread_id, OT_NULL);

    return ret;
}

static ot_s32 sample_venc_h265_mode(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg;
    ot_eis_img_size img_size = { 0 };
    sample_sns_type sns_type = g_vi_attr->sns_type[0];
    ot_eis_handle pool_hdl = OT_NULL;

    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    sample_comm_vi_get_size_by_sns_type(sns_type, &img_size);
    if (sample_vio_sys_init(&img_size) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    g_sample_venc_attr.venc_attr.common_attr.max_pic_width = img_size.width;
    g_sample_venc_attr.venc_attr.common_attr.max_pic_height = img_size.height;
    g_sample_venc_attr.venc_attr.common_attr.out_width = img_size.width;
    g_sample_venc_attr.venc_attr.common_attr.out_height = img_size.height;

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

    ret = sample_comm_venc_start(&g_sample_venc_attr);
    if (ret != OT_SUCCESS) {
        goto start_venc_failed;
    }

    ret = sample_vi_frame_to_venc_to_file(&vi_cfg, &g_sample_venc_attr);

    sample_comm_venc_stop(&g_sample_venc_attr);
start_venc_failed:
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_vi_user_mode_buf_pool_destroy(pool_hdl);
buf_pool_creat_failed:
    sample_comm_sys_exit();

    return ret;
}

static ot_s32 sample_venc_h264_mode(ot_void)
{
    g_sample_venc_attr.venc_attr.codecFormat = OT_EIS_VENC_CODEC_H264;
    g_sample_venc_attr.venc_attr.common_attr.max_pic_width = 1920;
    g_sample_venc_attr.venc_attr.common_attr.max_pic_height = 1080;
    g_sample_venc_attr.venc_attr.common_attr.out_width = 1920;
    g_sample_venc_attr.venc_attr.common_attr.out_height = 1080;
    // g_sample_venc_attr.venc_attr.common_attr.stream_buf_size = 1555200;

    return sample_venc_h265_mode();
}

static ot_s32 sample_venc_jpeg_mode(ot_void)
{
    g_sample_venc_attr.venc_attr.codecFormat = OT_EIS_VENC_CODEC_JPEG;
    g_sample_venc_attr.venc_attr.common_attr.max_pic_width = 1920;
    g_sample_venc_attr.venc_attr.common_attr.max_pic_height = 1080;
    g_sample_venc_attr.venc_attr.common_attr.out_width = 1920;
    g_sample_venc_attr.venc_attr.common_attr.out_height = 1080;
    // g_sample_venc_attr.venc_attr.common_attr.stream_buf_size = 2088960;

    return sample_venc_h265_mode();
}

static ot_void sample_venc_roi_attr_init(ot_eis_venc_video_roi_attr* roi_attr)
{
    roi_attr->is_abs_qp = OT_TRUE;
    roi_attr->enable = OT_TRUE;
    roi_attr->qp_val = 0; /* 30: qp value */
    roi_attr->index = 0;
    roi_attr->rect.x = 832;       /* 64: rect.x value */
    roi_attr->rect.y = 608;       /* 64: rect.y value */
    roi_attr->rect.height = 512; /* 256: rect.height value */
    roi_attr->rect.width = 512;  /* 256: rect.width value */
}

ot_s32 sample_comm_venc_set_roi(sample_venc_attr* attr)
{
    ot_s32 ret = OT_SUCCESS;
    ot_eis_venc_video_roi_attr roi_attr = { 0 };
    roi_attr.index = 0;

    if ((ret = ot_eis_venc_get_video_roi_attr(attr->venc_hdl, &roi_attr)) != OT_SUCCESS) { /* 0: roi index */
        sample_print("chn %d Get Roi Attr failed for %#x!\n", (ot_s32)((intptr_t)attr->venc_hdl), ret);
        return ret;
    }

    sample_venc_roi_attr_init(&roi_attr);

    if ((ret = ot_eis_venc_set_video_roi_attr(attr->venc_hdl, &roi_attr)) != OT_SUCCESS) {
        sample_print("chn %d Set Roi Attr failed for %#x!\n", (ot_s32)((intptr_t)attr->venc_hdl), ret);
        return ret;
    }

    return ret;
}

static ot_s32 sample_venc_h265_mode_set_roi()
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg;
    ot_eis_img_size img_size = { 0 };
    sample_sns_type sns_type = g_vi_attr->sns_type[0];
    ot_eis_handle pool_hdl = OT_NULL;

    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    sample_comm_vi_get_size_by_sns_type(sns_type, &img_size);
    if (sample_vio_sys_init(&img_size) != OT_SUCCESS) {
        return OT_FAILURE;
    }
    g_sample_venc_attr.venc_attr.common_attr.max_pic_width = img_size.width;
    g_sample_venc_attr.venc_attr.common_attr.max_pic_height = img_size.height;
    g_sample_venc_attr.venc_attr.common_attr.out_width = img_size.width;
    g_sample_venc_attr.venc_attr.common_attr.out_height = img_size.height;
    g_sample_venc_attr.venc_attr.common_attr.stream_buf_size = img_size.width * img_size.height * 1.5 * 6;
    g_sample_venc_attr.venc_attr.video_attr.rc_attr.cvbr_attr.max_bit_rate = 60000;
    g_sample_venc_attr.venc_attr.video_attr.rc_attr.cvbr_attr.long_max_bit_rate = 60000;
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

    ret = sample_comm_venc_start(&g_sample_venc_attr);
    if (ret != OT_SUCCESS) {
        goto start_venc_failed;
    }

    ret = sample_comm_venc_set_roi(&g_sample_venc_attr);
    if (ret != OT_SUCCESS) {
        goto set_roi_failed;
    }

    ret = sample_vi_frame_to_venc_to_file(&vi_cfg, &g_sample_venc_attr);

set_roi_failed:
    sample_comm_venc_stop(&g_sample_venc_attr);
start_venc_failed:
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_vi_user_mode_buf_pool_destroy(pool_hdl);
buf_pool_creat_failed:
    sample_comm_sys_exit();

    return ret;
}

//////////////////////////////////////////////////////////////////////////////


typedef struct {
    ot_eis_handle vproc_chn_hdl;
    ot_eis_handle venc_hdl;
} vproc_send_attr;
static const ot_eis_vproc_pipe_attr g_vproc_pipe_attr = {
    .mode = OT_EIS_VPROC_WORK_MODE_PIPE,
    .frc = {
        .src_frame_rate = -1,
        .dst_frame_rate = -1,
    },
    .image_attr = {
        .pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
        .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
        .compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    },
};

static sample_vproc_attr g_sample_vproc_attr = {
    // pipe param
    .pipe_attr[0] = g_vproc_pipe_attr,
    .pipe_attr[1] = g_vproc_pipe_attr,
    .pipe_attr[2] = g_vproc_pipe_attr,
    .pipe_attr[3] = g_vproc_pipe_attr,
    // group param
    .grp_attr = {
        .max_width = 3840,
        .max_height = 2160,
    },

    // channel param
    .chn_attr[0] = {
        .image_attr = {
            .width = 3840,
            .height = 2160,
            .pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
        },
        .mode = OT_EIS_VPROC_WORK_MODE_PIPE,
        .frc = {
            .src_frame_rate = -1,
            .dst_frame_rate = -1,
        },
        .frame_queue_depth = 0,
        .pool_handle = 0,
    },
    .chn_attr[1] = {
        .image_attr = {
            .width = 3840,
            .height = 2160,
            .pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
        },
        .mode = OT_EIS_VPROC_WORK_MODE_USER,
        .frc = {
            .src_frame_rate = -1,
            .dst_frame_rate = -1,
        },
        .frame_queue_depth = 6,
        .pool_handle = 0,
    },
    .chn_attr[2] = {
        .image_attr = {
            .width = 3840,
            .height = 2160,
            .pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
        },
        .mode = OT_EIS_VPROC_WORK_MODE_USER,
        .frc = {
            .src_frame_rate = -1,
            .dst_frame_rate = -1,
        },
        .frame_queue_depth = 6,
        .pool_handle = 0,
    },
    .chn_attr[3] = {
        .image_attr = {
            .width = 3840,
            .height = 2160,
            .pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
        },
        .mode = OT_EIS_VPROC_WORK_MODE_USER,
        .frc = {
            .src_frame_rate = -1,
            .dst_frame_rate = -1,
        },
        .frame_queue_depth = 6,
        .pool_handle = 0,
    },
    .is_set_attr = OT_FALSE,
    .set_attr = {
        .crop_param = {
            .enable = OT_FALSE,
            .crop_type = OT_EIS_COORD_ABS,
            .crop_rect = {0, 0, 1920, 1080},
        },
        .mirror_param = {
            .enable = OT_FALSE,
        },
        .flip_param = {
            .enable = OT_FALSE,
        },
        .mosaic_param = {
            .enable = OT_FALSE,
        },
        .line_param = {
            .enable = OT_FALSE,
        },
        .cover_param = {
            .enable = OT_FALSE,
        },
        .rect_param = {
            .enable = OT_FALSE,
        },
    }
};

/* get stream from each channels and save them */
ot_void* sample_vproc_venc_get_venc_stream_proc(ot_void* p)
{
    ot_s32 ret;

    prctl(PR_SET_NAME, "get_venc_stream", 0, 0, 0);

    // ret = sample_comm_set_name_save_stream(&g_sample_venc_attr);
    // if (ret != OT_SUCCESS) {
    //     sample_print("set name save stream failed\n");
    //     return OT_NULL;
    // }

    while (g_save_file_pthread == OT_TRUE) {
        ret = sample_comm_get_stream_from_one_channl(&g_sample_venc_attr);
        if (ret == 1) {
            sample_print("sample_comm_get_stream_from_one_channl chn[%d] JPEG done!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl));
            break;
        }

        if (ret != OT_SUCCESS) {
            sample_print("sample_comm_get_stream_from_one_channl chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl), ret);
        }
    }

    g_send_chn_pthread = OT_FALSE;
    // fclose(g_file);
    return OT_NULL;
}

static ot_void* sample_vproc_get_frame_send_to_venc_thread(ot_void* param)
{
    ot_s32 ret;
    ot_eis_handle vproc_chn_hdl;
    ot_eis_handle venc_hdl;
    vproc_send_attr* send_attr = (vproc_send_attr*)param;
    ot_eis_video_frame video_frame = { 0 };
    ot_eis_venc_image_frame venc_frame = { 0 };
    ot_s32 milli_sec = -1;

    vproc_chn_hdl = send_attr->vproc_chn_hdl;
    venc_hdl = send_attr->venc_hdl;
    while (g_send_chn_pthread) {
        ret = ot_eis_vproc_chn_acquire_frame(vproc_chn_hdl, &video_frame, milli_sec);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vi_chn_get_frame failed with %#x\n", ret);
            goto exit;
        }

        venc_frame.frame = &video_frame;
        ret = ot_eis_venc_send_frame(venc_hdl, &venc_frame, -1);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_venc_send_frame failed with %#x\n", ret);
            ot_eis_vproc_chn_release_frame(vproc_chn_hdl, &video_frame);
            goto exit;
        }

        ret = ot_eis_vproc_chn_release_frame(vproc_chn_hdl, &video_frame);
        if (ret != OT_SUCCESS) {
            goto exit;
        }
    }

exit:
    return OT_NULL;
}

static ot_s32 sample_vproc_frame_to_venc_to_file(sample_vproc_attr* vproc_attr, sample_venc_attr* venc_attr)
{
    ot_s32 ret;
    pthread_t thread_id = 0;
    pthread_t save_pid = 0;
    vproc_send_attr send_attr = { 0 };
    send_attr.vproc_chn_hdl = vproc_attr->chn_hdl[1];
    send_attr.venc_hdl = venc_attr->venc_hdl;

    g_send_chn_pthread = OT_TRUE;
    ret = pthread_create(&thread_id, OT_NULL, sample_vproc_get_frame_send_to_venc_thread, (ot_void*)&send_attr);
    if (ret != OT_SUCCESS) {
        printf("create send frame thread failed!\n");
        g_send_chn_pthread = OT_FALSE;
        return OT_FAILURE;
    }

    // 从venc取帧存入文件
    g_save_file_pthread = OT_TRUE;
    ret = pthread_create(&save_pid, OT_NULL, sample_vproc_venc_get_venc_stream_proc, OT_NULL);
    if (ret != OT_SUCCESS) {
        printf("create save frame thread failed!\n");
        g_save_file_pthread = OT_FALSE;
        return OT_FAILURE;
    }

    printf("threads running, print any key to exit!\n");
    getchar();

    g_save_file_pthread = OT_FALSE;
    g_send_chn_pthread = OT_FALSE;

    pthread_join(save_pid, OT_NULL);
    pthread_join(thread_id, OT_NULL);

    sample_print("sample_venc_chn[%d] success get %lld frame streams!\n", (ot_s32)((intptr_t)venc_attr->venc_hdl), g_jpeg_acquire_cnt);

    return ret;
}

static ot_s32 sample_vproc_user_mode_buf_pool_creat(ot_eis_img_attr *img_attr, ot_eis_handle *pool_hdl)
{
    ot_s32 ret;
    ot_u32 block_size;
    ot_video_buffer_attr pool_attr = { 0 };
    ot_eis_handle tmp;

    img_attr->bit_depth = OT_EIS_PIXEL_BIT_DEPTH_8;
    sample_common_get_buffer_pool_cfg(img_attr, &block_size);
    pool_attr.cnt = 1;
    pool_attr.buf_blks[0].cnt = 10;
    pool_attr.buf_blks[0].size = block_size;

    ret = ot_buffer_pool_init();
    if (ret != 0) {
        printf("buffer_pool init fail! ret[%d]\n", ret);
        return OT_FAILURE;
    }

    ret = ot_buffer_pool_create(&tmp, &pool_attr);
    if (ret != 0) {
        printf("buffer_pool create fail! ret[%d]\n", ret);
        goto pool_create_failed;
    }
    *pool_hdl = tmp;
    sample_print("create pool hdl 0x%llx \n", (ot_u64)tmp);
    return OT_SUCCESS;

pool_create_failed:
    ot_buffer_pool_deinit();
    return ret;
}

static ot_void sample_vproc_user_mode_buf_pool_destroy(ot_eis_handle pool_hdl)
{
    sample_print("destroy pool hdl 0x%llx \n", (ot_u64)pool_hdl);
    ot_buffer_pool_destroy(pool_hdl);
    ot_buffer_pool_deinit();
}
static ot_s32 sample_vio_vproc_for_user_mode(sample_vproc_attr* vproc_attr, ot_bool chn_sw[])
{
    ot_s32 ret = OT_SUCCESS;

    int j;

    for (j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
        if ((chn_sw[j] == OT_TRUE) && (vproc_attr->chn_attr[j].mode == OT_EIS_VPROC_WORK_MODE_USER)) {
            break;
        }
    }
    if (j >= OT_EIS_VPROC_GRP_CHN_MAX_NUM) {
        sample_print("no match pipe_sw or chn_sw for send frame\n");
        return OT_FAILURE;
    }

    ot_eis_handle pool_hdl;
    ot_eis_img_attr buf_attr;
    buf_attr.width         = vproc_attr->chn_attr[j].image_attr.width;
    buf_attr.height        = vproc_attr->chn_attr[j].image_attr.height;
    buf_attr.layout        = OT_EIS_IMAGE_LAYOUT_LINEAR;
    buf_attr.bit_depth     = OT_EIS_PIXEL_BIT_DEPTH_8;
    buf_attr.pixel_fmt     = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR; //vproc_attr->chn_attr[j].image_attr.pixel_fmt;
    buf_attr.compress_mode = vproc_attr->chn_attr[j].image_attr.compress_mode;

    ot_u32 blk_cnt = 20;
    ret = sample_comm_media_create_buf_pool(&pool_hdl, &buf_attr, blk_cnt);
    if (ret != OT_SUCCESS)
    {
        return ret;
    }

    vproc_attr->chn_attr[j].pool_handle = pool_hdl;

    return ret;
}
static ot_s32 sample_venc_h265_mode_from_vproc(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type = g_vi_attr->sns_type[0];
    ot_eis_handle pool_hdl = OT_NULL;
    ot_eis_img_size img_size = { 0 };
    ot_eis_handle media_pipe_hdl;

    //根据sensor类型获取图片大小
    sample_comm_vi_get_size_by_sns_type(sns_type, &img_size);

    // ot_eis_handle media_pipe_hdl;
    ot_bool pipe_sw[OT_EIS_VPROC_GRP_PIPE_MAX_NUM] = {OT_TRUE, OT_FALSE, OT_FALSE, OT_FALSE};
    ot_bool chn_sw[OT_EIS_VPROC_GRP_CHN_MAX_NUM] = {OT_FALSE, OT_TRUE, OT_FALSE, OT_FALSE};

    //获取VI默认配置
    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    //获取vproc默认配置
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));
    if (sample_vio_sys_init(&img_size) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    //创建vproc捞帧所用的buf_pool
    ret = sample_vproc_user_mode_buf_pool_creat(&g_sample_vproc_attr.chn_attr[1].image_attr, &pool_hdl);
    if (ret != OT_SUCCESS) {
        goto buf_pool_creat_failed;
    }
    g_sample_vproc_attr.chn_attr[1].pool_handle = pool_hdl;
    sample_print("sample_vproc_user_mode_buf_pool_creat pool hdl 0x%llx \n", (ot_u64)pool_hdl);


    //开始VI模块
    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != OT_SUCCESS) {
        goto start_vi_failed;
    }

    // 开启VPROC模块
    ret = sample_comm_start_vproc(&g_sample_vproc_attr, pipe_sw, chn_sw);
    if (ret != OT_SUCCESS) {
        goto start_vproc_failed;
    }

    //初始化pipe，并绑定VI -> VPROC
    ret = sample_comm_media_pipe_init(&media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto pipe_start_failed;
    }
    ret = sample_comm_vi_bind_vproc(vi_cfg.pipe_info[0].chn_info.chn_hdl, g_sample_vproc_attr.pipe_hdl[0], media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto vi_vproc_bind_failed;
    }

    //开启VENC模块
    g_sample_venc_attr.venc_attr.common_attr.max_pic_width = g_sample_vproc_attr.chn_attr[1].image_attr.width;
    g_sample_venc_attr.venc_attr.common_attr.max_pic_height = g_sample_vproc_attr.chn_attr[1].image_attr.height;
    g_sample_venc_attr.venc_attr.common_attr.out_width = g_sample_vproc_attr.chn_attr[1].image_attr.width;
    g_sample_venc_attr.venc_attr.common_attr.out_height = g_sample_vproc_attr.chn_attr[1].image_attr.height;
    ret = sample_comm_venc_start(&g_sample_venc_attr);
    if (ret != OT_SUCCESS) {
        goto start_venc_failed;
    }

    ret = sample_vproc_frame_to_venc_to_file(&g_sample_vproc_attr, &g_sample_venc_attr);

    sample_comm_venc_stop(&g_sample_venc_attr);


start_venc_failed:
    sample_comm_vi_un_bind_vproc(vi_cfg.pipe_info[0].chn_info.chn_hdl, g_sample_vproc_attr.pipe_hdl[0], media_pipe_hdl);
vi_vproc_bind_failed:
    sample_comm_media_pipe_stop(media_pipe_hdl);
pipe_start_failed:
    sample_comm_stop_vproc(&g_sample_vproc_attr);
start_vproc_failed:
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_vproc_user_mode_buf_pool_destroy(pool_hdl);
buf_pool_creat_failed:
    sample_comm_sys_exit();

    return ret;
}
static ot_s32 sample_venc_h264_mode_from_vproc(ot_void)
{
    g_sample_venc_attr.venc_attr.codecFormat = OT_EIS_VENC_CODEC_H264;
    return sample_venc_h265_mode_from_vproc();
}
//////////////////////////////////////////////////////////////////////////////

static ot_void sample_venc_usage(const char* prg_name)
{
    printf("usage : %s <index> -i <sensor_type>\n", prg_name);
    printf("index:\n");
    printf("    (0) H265 vi -> venc -> file\n");
    printf("    (1) H264 vi -> venc -> file\n");
    printf("    (2) jpeg vi -> venc -> file\n");
    printf("    (3) H265(roi) vi -> venc -> file\n");
    printf("    (4) H265 vi -> vproc -> venc -> file\n");
    printf("    (5) H264 vi -> vproc -> venc -> file\n");
    sample_comm_vi_args_help();
}

static ot_s32 sample_venc_check_params(sample_param_comb* param_comb)
{
    if (param_comb->input_param > ONE_MIPI_ONE_SNS_MAX) {
        sample_print("Don't support multi sensor venc.\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_void sample_venc_handle_sig(ot_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        g_sig_flag = 1;
    }
}

static ot_void sample_register_sig_handler(ot_void(*sig_handle)(ot_s32))
{
    struct sigaction sa;

    (ot_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handle;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, OT_NULL);
    sigaction(SIGTERM, &sa, OT_NULL);
}

static ot_s32 sample_venc_execute_case(ot_u32 case_index)
{
    ot_s32 ret;

    switch (case_index) {
    case 0:
        ret = sample_venc_h265_mode();
        break;
    case 1:
        ret = sample_venc_h264_mode();
        break;
    case 2:
        ret = sample_venc_jpeg_mode();
        break;
    case 3:
        ret = sample_venc_h265_mode_set_roi();
        break;
    case 4:
        ret = sample_venc_h265_mode_from_vproc();
        break;
    case 5:
        ret = sample_venc_h264_mode_from_vproc();
        break;
    default:
        ret = OT_FAILURE;
        break;
    }

    return ret;
}

ot_s32 main(ot_s32 argc, ot_char* argv[])
{
    ot_s32 ret;
    ot_u32 index;
    ot_u32 sensor_type;
    sample_param_comb* param_comb = OT_NULL;

   if ((argc < 2) || (argc > 6)) { /* 2:arg num */
        sample_venc_usage(argv[0]);
        return OT_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_venc_usage(argv[0]);
        return OT_FAILURE;
    }

    if (strlen(argv[1]) > 2 || strlen(argv[1]) <= 0 || !check_digit(argv[1][0]) || /* 2:arg len */
        (strlen(argv[1]) == 2 && (!check_digit(argv[1][1]) || argv[1][0] == '0'))) { /* 2:arg len */
        sample_venc_usage(argv[0]);
        return OT_FAILURE;
    }

    if (strlen(argv[1]) == 2 && argv[1][1] != '0') { /* 2:arg len, max: 10 */
        sample_venc_usage(argv[0]);
        return OT_FAILURE;
    }

    sample_register_sig_handler(sample_venc_handle_sig);

    index = strtol(argv[1], NULL, 10); /* base 10 */
    ret = sample_comm_args_register(argc, argv);
    if (ret != OT_SUCCESS) {
        sample_venc_usage(argv[0]);
        return OT_FAILURE;
    }

    param_comb = sample_comm_args_get_param_comb();
    ret = sample_venc_check_params(param_comb);
    if (ret != OT_SUCCESS) {
        sample_venc_usage(argv[0]);
        return OT_FAILURE;
    }

    ret = sample_venc_execute_case(index);
    if ((ret == OT_SUCCESS) && (g_sig_flag == 0)) {
        printf("\033[0;32mprogram exit normally!\033[0;39m\n");
    }
    else {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    return ret;
}
