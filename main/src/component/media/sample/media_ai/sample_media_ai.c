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
#include "ot_avp_npu_rts.h"
#include "ot_avp_cve.h"
#include <math.h>
#include "ot_avp_cve_alg_lib.h"
#include "sample_ai.h"


#define ENABLE_VI_VPROC_VO 1

static volatile sig_atomic_t g_sig_flag = 0;
ot_eis_vi_vproc_mode_type g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;

typedef struct {
    ot_eis_handle pipe_hdl[OT_EIS_VPROC_GRP_PIPE_MAX_NUM];
    ot_eis_vproc_pipe_attr pipe_attr[OT_EIS_VPROC_GRP_PIPE_MAX_NUM];

    ot_eis_handle grp_hdl[OT_EIS_VPROC_GRP_MAX_NUM];
    ot_eis_vproc_grp_attr grp_attr[OT_EIS_VPROC_GRP_MAX_NUM];

    ot_eis_handle chn_hdl[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_eis_vproc_chn_attr chn_attr[OT_EIS_VPROC_GRP_CHN_MAX_NUM];

    ot_bool is_set_attr;
    sample_vproc_set_attr set_attr;

    ot_eis_handle pool_hdl[2];
} sample_media_ai_vproc_attr;

static sample_media_ai_vproc_attr g_sample_vproc_attr = {0};

static ot_bool g_send_chn_pthread = OT_FALSE;

static ot_void sample_get_char(ot_void)
{
    if (g_sig_flag == 1) {
        return;
    }

    sample_pause();
}

static ot_s32 sample_vio_sys_init(ot_eis_img_size *img_size)
{
    ot_video_buffer_attr vrb_cfg;

    sample_common_get_vrb_cfg(3, img_size->width, img_size->height, &vrb_cfg);
    if (sample_comm_sys_init(&vrb_cfg) != OT_SUCCESS) {
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
    if (ret != 0 && ret != OT_ERR_BUFPOOL_ALREADY_INIT) {
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

ot_void sample_npu_scale_coords()
{
    sample_npu_attr* npu_attr =  get_sample_npu_attr();
    float src_width = 640;
    float src_height = 640;
    float dst_width = 1280;
    float dst_height = 720;
    for (ot_u32 i = 0; i < npu_attr->bbox_num; i++) {
        ot_eis_img_rect *ori_src = &npu_attr->vproc_rect[i].rect;
        // {x1y1x2y2}
        float src[4] = {ori_src->x, ori_src->y, ori_src->x + ori_src->width, ori_src->y + ori_src->height};
        float gain_width = dst_width / src_width;
        float gain_height = dst_height / src_height;

        src[0] *= gain_width;
        src[2] *= gain_width;
        src[1] *= gain_height;
        src[3] *= gain_height;
        for (ot_u32 j = 0; j < 4; j++) {
            src[j] = MAX(src[j], 0);
        }

        src[0] = MIN(src[0], dst_width);
        src[2] = MIN(src[2], dst_width);

        src[1] = MIN(src[1], dst_height);
        src[3] = MIN(src[3], dst_height);

        // 2对齐处理
        ori_src->x = src[0];
        ori_src->x = ori_src->x / 2 * 2;
        ori_src->y = src[1];
        ori_src->y = ori_src->y / 2 * 2;

        ori_src->width = src[2] - src[0];
        ori_src->width = ori_src->width / 2 * 2;
        ori_src->height = src[3] - src[1];
        ori_src->height = ori_src->height / 2 * 2;
    }

}

ot_void sample_ai_set_osd()
{
    sample_npu_attr* npu_attr =  get_sample_npu_attr();

    for (ot_u32 i = 0; i < npu_attr->bbox_num; i++) {
        ot_avp_img_rect *npu_rect = &npu_attr->rect[i];
        ot_eis_img_rect *vproc_rect = &npu_attr->vproc_rect[i].rect;

        ot_eis_osd_attr *osd_attr = &npu_attr->osd_attr[i];
        osd_attr->osd_rgn.x = vproc_rect->x;
        osd_attr->osd_rgn.y = vproc_rect->y;
        osd_attr->osd_rgn.width = 64;
        osd_attr->osd_rgn.height = 32;

        osd_attr->color = 0xff;
        if (npu_rect->classId == 0) {   // person
            osd_attr->osd_frame = &npu_attr->osd_fram[0];
        } else if (npu_rect->classId == 2)  { // car
            osd_attr->osd_frame = &npu_attr->osd_fram[1];
        } else {    // unknown
            osd_attr->osd_frame = &npu_attr->osd_fram[2];
        }

        for (ot_u32 i = 0; i < OT_EIS_VPROC_OSD_LUT_NUM; i++) {
            osd_attr->alpha_lut[i] = (i + 1) * (256 / OT_EIS_VPROC_OSD_LUT_NUM) - 1;
        }
    }
}

static ot_u64 g_cal_fps_start = 0;
static ot_u64 g_frame_cnt = 0;
static ot_void sample_media_ai_calu_fps()
{
    const ot_u32 send_frame_times = 100;
    if (g_frame_cnt == 0) {
        g_cal_fps_start = sample_ai_get_time_ms();
        g_frame_cnt++;
    } else if (g_frame_cnt == send_frame_times) {
        ot_u64 cal_fps_end = sample_ai_get_time_ms();
        double fps = 1000 / ((cal_fps_end - g_cal_fps_start) / send_frame_times);
        g_frame_cnt = 0;
        printf("-----------  fps : %lf -------------\n", fps);
    } else {
        g_frame_cnt++;
    }
}

static ot_void* sample_vproc_get_frame_to_ai_thread()
{
    ot_s32 ret;
    ot_eis_handle vproc_chn_hdl[3];
    ot_eis_video_frame video_frame[2] = { 0 };
    ot_s32 milli_sec = -1;
    ot_u32 no_target_cnt = 0;

    vproc_chn_hdl[0] = g_sample_vproc_attr.chn_hdl[0];
    vproc_chn_hdl[1] = g_sample_vproc_attr.chn_hdl[1];
    vproc_chn_hdl[2] = g_sample_vproc_attr.chn_hdl[2];
    while (g_send_chn_pthread) {
        ret = ot_eis_vproc_chn_acquire_frame(vproc_chn_hdl[0], &video_frame[0], milli_sec);  // get 1280*720s
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vi_chn_get_frame failed with %#x\n", ret);
            goto exit;
        }
        ret = ot_eis_vproc_chn_acquire_frame(vproc_chn_hdl[1], &video_frame[1], milli_sec); // get 640*640
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vi_chn_get_frame failed with %#x\n", ret);
            ot_eis_vproc_chn_release_frame(vproc_chn_hdl[0], &video_frame[0]);
            goto exit;
        }

        /*----------- ai proc ----------*/
        // ot_u64 start_time = sample_ai_get_time_ms();
        sample_ai_reset_data();
        // vproc-> npu yolov5, 更新输入(vproc_chn_2_video_frame -> yolov5 input_dataset), 输出作为缓存复用。
        sample_npu_attr* npu_attr =  get_sample_npu_attr();

        ot_void *virt_addr = OT_NULL;
        ot_smr_mmap(video_frame[1].buff.phys_addr[0], npu_attr->yolov5_model_info.input_buffsize[0], OT_FALSE, &virt_addr);
        ot_avp_npu_set_buffer(npu_attr->yolov5_model_info.input_dataset, 0, virt_addr, 0,
            npu_attr->yolov5_model_info.input_buffsize[0], npu_attr->yolov5_model_info.input_stride[0]);
        ret = ot_avp_npu_execute(npu_attr->yolov5_model_hdl, npu_attr->yolov5_model_info.input_dataset, npu_attr->yolov5_model_info.output_dataset);
        if (ret != 0) {
            printf("npu execute fail.\n");
        }
        // printf("yolov5 execute time =%llu ms\n", sample_ai_get_time_ms() - start_time);
        sample_npu_yolov5_data_trans(); // yolov5后处理, 得到矩形框
        // printf("yolov5 postprocess time =%llu ms\n", sample_ai_get_time_ms() - start_time);
        if (npu_attr->bbox_num > 0) {
            // cve corp and resize
            sample_cve_crop_and_resize(&video_frame[1]);
            // printf("cve crop & resize time =%llu ms\n", sample_ai_get_time_ms() - start_time);
            sample_npu_mobilenetv2_execute(); // mobilnetv2-12

            // vproc画框, 更新框的参数
            g_sample_vproc_attr.set_attr.rect_param.enable = OT_TRUE;
            g_sample_vproc_attr.set_attr.rect_param.num = npu_attr->bbox_num;
            g_sample_vproc_attr.set_attr.rect_param.rect = npu_attr->vproc_rect;

            // 更新osd的参数
            g_sample_vproc_attr.set_attr.osd_param.enable = OT_TRUE;
            g_sample_vproc_attr.set_attr.osd_param.num = npu_attr->bbox_num;
            g_sample_vproc_attr.set_attr.osd_param.osd = npu_attr->osd_attr;

            sample_npu_scale_coords();
            sample_ai_set_osd();

            ret = ot_eis_vproc_chn_set_osd(vproc_chn_hdl[2], &g_sample_vproc_attr.set_attr.osd_param);
            ret = ot_eis_vproc_chn_set_rect(vproc_chn_hdl[2], &g_sample_vproc_attr.set_attr.rect_param);

            // ret = ot_eis_vproc_chn_set_osd(vproc_chn_hdl[0], &g_sample_vproc_attr.set_attr.osd_param);
            // ret = ot_eis_vproc_chn_set_rect(vproc_chn_hdl[0], &g_sample_vproc_attr.set_attr.rect_param);
            no_target_cnt = 0;
        } else {
            no_target_cnt++;
            if (g_sample_vproc_attr.set_attr.rect_param.enable && no_target_cnt > 20) {
                g_sample_vproc_attr.set_attr.rect_param.enable = OT_FALSE;
                ret = ot_eis_vproc_chn_set_rect(vproc_chn_hdl[2], &g_sample_vproc_attr.set_attr.rect_param);
                g_sample_vproc_attr.set_attr.osd_param.enable = OT_FALSE;
                ret = ot_eis_vproc_chn_set_osd(vproc_chn_hdl[2], &g_sample_vproc_attr.set_attr.osd_param);
            }
        }
        ot_smr_munmap(virt_addr, npu_attr->yolov5_model_info.input_buffsize[0]);
        // printf("ai proc total time =%llu ms\n", sample_ai_get_time_ms() - start_time);

        // 释放vproc的输入内存
        ot_eis_vproc_chn_release_frame(vproc_chn_hdl[1], &video_frame[1]);

        // send to vo
        ot_eis_vproc_pipe_send_frame(g_sample_vproc_attr.pipe_hdl[1], &video_frame[0], 1);
        ot_eis_vproc_chn_release_frame(vproc_chn_hdl[0], &video_frame[0]);
        /*----------- ai proc ----------*/

        sample_media_ai_calu_fps();
    }

exit:
    return OT_NULL;
}

static ot_s32 sample_vproc_frame_to_ai()
{
    ot_s32 ret;
    pthread_t thread_id = 0;
    g_send_chn_pthread = OT_TRUE;
    ret = pthread_create(&thread_id, OT_NULL, sample_vproc_get_frame_to_ai_thread, OT_NULL);
    if (ret != OT_SUCCESS) {
        printf("create send frame thread failed!\n");
        g_send_chn_pthread = OT_FALSE;
        return OT_FAILURE;
    }
    printf("threads running, print any key to exit!\n");
    getchar();

    g_send_chn_pthread = OT_FALSE;
    pthread_join(thread_id, OT_NULL);

    return ret;
}

static ot_s32 sample_media_ai_offline(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type = SENSOR0_TYPE;
    ot_eis_handle pool_hdl[2] = {0}; // 640x640,1280x720
    ot_eis_img_size img_size = {0};
    sample_vo_attr *vo_attr = OT_NULL;
    ot_eis_handle media_pipe_hdl;
    ot_eis_handle hdls[4];
    ot_s32 num;

    sample_comm_vi_get_size_by_sns_type(SENSOR0_TYPE, &img_size);
    vo_attr = sample_comm_get_vo_attr(ONE_IN_PORT_MIPI_768);
    if (vo_attr == OT_NULL) {
        sample_print("Invalid vo attr.\n");
        return OT_FAILURE;
    }

    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    if (sample_vio_sys_init(&img_size) != OT_SUCCESS) {
        return OT_FAILURE;
    }
    // ai
    ret = sample_ai_init();
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, ai_init_failed, "sample_ai_init fail.\n");
    ret = sample_ai_create_data();
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, ai_create_data_failed, "sample_ai_create_data fail.\n");

    ret = ot_eis_vproc_init();
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_init_failed, "ot_eis_vproc_init fail.\n");

    // vps
    g_sample_vproc_attr.chn_attr[0].image_attr.width  = 1280;  // Vproc.cropAndResize 1280x720
    g_sample_vproc_attr.chn_attr[0].image_attr.height = 720;
    g_sample_vproc_attr.chn_attr[0].image_attr.pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
    g_sample_vproc_attr.chn_attr[0].image_attr.layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
    g_sample_vproc_attr.chn_attr[0].image_attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    g_sample_vproc_attr.chn_attr[0].frc.src_frame_rate = -1;
    g_sample_vproc_attr.chn_attr[0].frc.dst_frame_rate = -1;
    g_sample_vproc_attr.chn_attr[0].frame_queue_depth = 6;
    g_sample_vproc_attr.chn_attr[0].mode = OT_EIS_VPROC_WORK_MODE_USER;

    g_sample_vproc_attr.chn_attr[1].image_attr.width  = 640;    // Vproc.cropAndResize 640x640
    g_sample_vproc_attr.chn_attr[1].image_attr.height = 640;
    g_sample_vproc_attr.chn_attr[1].image_attr.pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
    g_sample_vproc_attr.chn_attr[1].image_attr.layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
    g_sample_vproc_attr.chn_attr[1].image_attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    g_sample_vproc_attr.chn_attr[1].frc.src_frame_rate = -1;
    g_sample_vproc_attr.chn_attr[1].frc.dst_frame_rate = -1;
    g_sample_vproc_attr.chn_attr[1].frame_queue_depth = 6;
    g_sample_vproc_attr.chn_attr[1].mode = OT_EIS_VPROC_WORK_MODE_USER;

    g_sample_vproc_attr.chn_attr[2].image_attr.width  = 1280;    // Vproc.cropAndResize 1280x720
    g_sample_vproc_attr.chn_attr[2].image_attr.height = 720;
    g_sample_vproc_attr.chn_attr[2].image_attr.pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
    g_sample_vproc_attr.chn_attr[2].image_attr.layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
    g_sample_vproc_attr.chn_attr[2].image_attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    g_sample_vproc_attr.chn_attr[2].frc.src_frame_rate = -1;
    g_sample_vproc_attr.chn_attr[2].frc.dst_frame_rate = -1;
    g_sample_vproc_attr.chn_attr[2].frame_queue_depth = 6;
    g_sample_vproc_attr.chn_attr[2].mode = OT_EIS_VPROC_WORK_MODE_USER;


    g_sample_vproc_attr.pipe_attr[0].mode = OT_EIS_VPROC_WORK_MODE_PIPE;
    g_sample_vproc_attr.pipe_attr[0].image_attr = g_sample_vproc_attr.chn_attr[0].image_attr;
    g_sample_vproc_attr.pipe_attr[0].frc.src_frame_rate = -1;
    g_sample_vproc_attr.pipe_attr[0].frc.dst_frame_rate = -1;

    g_sample_vproc_attr.pipe_attr[1].mode = OT_EIS_VPROC_WORK_MODE_USER;
    g_sample_vproc_attr.pipe_attr[1].image_attr = g_sample_vproc_attr.chn_attr[1].image_attr;
    g_sample_vproc_attr.pipe_attr[1].frc.src_frame_rate = -1;
    g_sample_vproc_attr.pipe_attr[1].frc.dst_frame_rate = -1;

    g_sample_vproc_attr.grp_attr[0].max_width = 1936;
    g_sample_vproc_attr.grp_attr[0].max_height = 1552;
    g_sample_vproc_attr.grp_attr[0].pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR;

    g_sample_vproc_attr.grp_attr[1].max_width = 1280;
    g_sample_vproc_attr.grp_attr[1].max_height = 720;
    g_sample_vproc_attr.grp_attr[1].pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR;

    ret = sample_vproc_user_mode_buf_pool_creat(&g_sample_vproc_attr.chn_attr[0].image_attr, &pool_hdl[0]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, buf_pool_0_creat_failed, "buf_pool_0 create fail.\n");
    ret = sample_vproc_user_mode_buf_pool_creat(&g_sample_vproc_attr.chn_attr[1].image_attr, &pool_hdl[1]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, buf_pool_1_creat_failed, "buf_pool_1 create fail.\n");
    g_sample_vproc_attr.chn_attr[0].pool_handle = pool_hdl[0];
    g_sample_vproc_attr.chn_attr[1].pool_handle = pool_hdl[1];

    // 第一个vproc
    ret = ot_eis_vproc_grp_create(0, &g_sample_vproc_attr.grp_hdl[0], &g_sample_vproc_attr.grp_attr[0]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_0_create_failed, "vproc grp 0 create fail.\n");
    ret = ot_eis_vproc_grp_start(g_sample_vproc_attr.grp_hdl[0]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_0_create_failed, "vproc grp 0 start fail.\n");

    ret = ot_eis_vproc_pipe_create(&g_sample_vproc_attr.pipe_hdl[0], &g_sample_vproc_attr.pipe_attr[0]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_0_create_failed, "vproc grp 0, pipe create fail.\n");
    ret = ot_eis_vproc_grp_attach_pipe(g_sample_vproc_attr.grp_hdl[0], g_sample_vproc_attr.pipe_hdl[0]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_0_create_failed, "vproc grp 0, pipe attach fail.\n");

    ret = ot_eis_vproc_chn_create(0, &g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.chn_attr[0]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_0_create_failed, "vproc grp 0, chn0 create fail.\n");
    ret = ot_eis_vproc_grp_attach_chn(g_sample_vproc_attr.grp_hdl[0], g_sample_vproc_attr.chn_hdl[0]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_0_create_failed, "vproc grp 0, chn0 attach fail.\n");
    ret = ot_eis_vproc_chn_create(1, &g_sample_vproc_attr.chn_hdl[1], &g_sample_vproc_attr.chn_attr[1]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_0_create_failed, "vproc grp 0, chn1 create fail.\n");
    ret = ot_eis_vproc_grp_attach_chn(g_sample_vproc_attr.grp_hdl[0], g_sample_vproc_attr.chn_hdl[1]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_0_create_failed, "vproc grp 0, chn1 attach fail.\n");

    // 第二个vproc
    ret = ot_eis_vproc_grp_create(1, &g_sample_vproc_attr.grp_hdl[1], &g_sample_vproc_attr.grp_attr[1]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_1_create_failed, "vproc grp 1 create fail.\n");
    ret = ot_eis_vproc_grp_start(g_sample_vproc_attr.grp_hdl[1]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_1_create_failed, "vproc grp 1 start fail.\n");

    ret = ot_eis_vproc_pipe_create(&g_sample_vproc_attr.pipe_hdl[1], &g_sample_vproc_attr.pipe_attr[1]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_1_create_failed, "vproc grp 1, pipe create fail.\n");
    ret = ot_eis_vproc_grp_attach_pipe(g_sample_vproc_attr.grp_hdl[1], g_sample_vproc_attr.pipe_hdl[1]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_1_create_failed, "vproc grp 1, pipe attach fail.\n");

    ret = ot_eis_vproc_chn_create(0, &g_sample_vproc_attr.chn_hdl[2], &g_sample_vproc_attr.chn_attr[2]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_1_create_failed, "vproc grp 1, chn2 create fail.\n");
    ret = ot_eis_vproc_grp_attach_chn(g_sample_vproc_attr.grp_hdl[1], g_sample_vproc_attr.chn_hdl[2]);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_1_create_failed, "vproc grp 1, chn2 attach fail.\n");

    // vi
    ret = sample_comm_vi_start_vi(&vi_cfg);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, start_vi_failed, "start_vi_failed fail.\n");

    // vo
    ret = sample_comm_start_vo(vo_attr);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, start_vo_failed, "start_vo_failed fail.\n");

    ret = sample_comm_media_pipe_init(&media_pipe_hdl);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, pipe_start_failed, "sample_comm_media_pipe_init fail.\n");

    // bind vi -> vproc
    ret = sample_comm_vi_bind_vproc(vi_cfg.pipe_info[0].chn_info.chn_hdl, g_sample_vproc_attr.pipe_hdl[0], media_pipe_hdl);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vi_vproc_bind_failed, "sample_comm_vi_bind_vproc fail.\n");

    // bind vproc -> vo
    // ret = sample_comm_vproc_bind_vo(g_sample_vproc_attr.chn_hdl[0], vo_attr->in_port_handle, media_pipe_hdl);
    // CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_bind_vo_failed, "sample_comm_vproc_bind_vo fail.\n");

    ret = sample_comm_vproc_bind_vo(g_sample_vproc_attr.chn_hdl[2], vo_attr->in_port_handle[0], media_pipe_hdl);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, vproc_bind_vo_failed, "sample_comm_vproc_bind_vo fail.\n");

    // process ai
    ret = sample_vproc_frame_to_ai();

    sample_get_char();


vproc_bind_vo_failed:
    sample_comm_vi_un_bind_vproc(vi_cfg.pipe_info[0].chn_info.chn_hdl, g_sample_vproc_attr.pipe_hdl[0], media_pipe_hdl);
vi_vproc_bind_failed:
    sample_comm_media_pipe_stop(media_pipe_hdl);
pipe_start_failed:
    sample_comm_stop_vo(vo_attr);
start_vo_failed:
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
vproc_1_create_failed:
    ot_eis_vproc_grp_stop(g_sample_vproc_attr.grp_hdl[1]);
    ret = ot_eis_vproc_grp_get_attched_pipes(g_sample_vproc_attr.grp_hdl[1], hdls, &num);
    for (int i = 0; i < num; i++) {
        ret = ot_eis_vproc_grp_detach_pipe(g_sample_vproc_attr.grp_hdl[1], hdls[i]);
        ret = ot_eis_vproc_pipe_destroy(hdls[i]);
    }
    ot_eis_vproc_grp_get_attched_chns(g_sample_vproc_attr.grp_hdl[1], hdls, &num);
    for (int i = 0; i < num; i++) {
        ret = ot_eis_vproc_grp_detach_chn(g_sample_vproc_attr.grp_hdl[1], hdls[i]);
        ret = ot_eis_vproc_chn_destroy(hdls[i]);
    }
    ot_eis_vproc_grp_destroy(g_sample_vproc_attr.grp_hdl[1]);
vproc_0_create_failed:
    ot_eis_vproc_grp_stop(g_sample_vproc_attr.grp_hdl[0]);
    ret = ot_eis_vproc_grp_get_attched_pipes(g_sample_vproc_attr.grp_hdl[0], hdls, &num);
    for (int i = 0; i < num; i++) {
        ret = ot_eis_vproc_grp_detach_pipe(g_sample_vproc_attr.grp_hdl[0], hdls[i]);
        ret = ot_eis_vproc_pipe_destroy(hdls[i]);
    }
    ot_eis_vproc_grp_get_attched_chns(g_sample_vproc_attr.grp_hdl[0], hdls, &num);
    for (int i = 0; i < num; i++) {
        ret = ot_eis_vproc_grp_detach_chn(g_sample_vproc_attr.grp_hdl[0], hdls[i]);
        ret = ot_eis_vproc_chn_destroy(hdls[i]);
    }
    ot_eis_vproc_grp_destroy(g_sample_vproc_attr.grp_hdl[0]);
    sample_vproc_user_mode_buf_pool_destroy(pool_hdl[1]);
buf_pool_1_creat_failed:
    sample_vproc_user_mode_buf_pool_destroy(pool_hdl[0]);
buf_pool_0_creat_failed:
    ot_eis_vproc_deinit();
vproc_init_failed:
    sample_ai_destory_data();
ai_create_data_failed:
    sample_ai_deinit();
ai_init_failed:
    sample_comm_sys_exit();

    return ret;
}


static ot_void sample_media_ai_usage(const char *prg_name)
{
    printf("usage : %s <index> \n", prg_name);
    printf("index:\n");
    printf("    (0) media_ai mode : vi -> vproc -> npu -> cve -> npu -> vproc -> vo.\n");
}

static ot_void sample_media_ai_handle_sig(ot_s32 signo)
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

static ot_s32 sample_media_ai_execute_case(ot_u32 case_index)
{
    ot_s32 ret;
    switch (case_index) {
        case 0: /* vi -> vproc -> npu -> cve -> npu -> vproc -> vo */
            ret = sample_media_ai_offline();
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
    ot_u32 index = 0;

    if (argc != 2) { /* 2:arg num */
        sample_media_ai_usage(argv[0]);
        return OT_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_media_ai_usage(argv[0]);
        return OT_FAILURE;
    }

    if (strlen(argv[1]) > 2 || strlen(argv[1]) <= 0 || !check_digit(argv[1][0]) || /* 2:arg len */
        (strlen(argv[1]) == 2 && (!check_digit(argv[1][1]) || argv[1][0] == '0'))) { /* 2:arg len */
        sample_media_ai_usage(argv[0]);
        return OT_FAILURE;
    }

    if (strlen(argv[1]) == 2 && argv[1][1] != '0') { /* 2:arg len, max: 10 */
        sample_media_ai_usage(argv[0]);
        return OT_FAILURE;
    }

    sample_register_sig_handler(sample_media_ai_handle_sig);

    index = strtol(argv[1], NULL, 10); /* base 10 */

    ret = sample_media_ai_execute_case(index);
    if ((ret == OT_SUCCESS) && (g_sig_flag == 0)) {
        printf("\033[0;32mprogram exit normally!\033[0;39m\n");
    } else {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    return ret;
}
